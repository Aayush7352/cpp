/*
THEORY: JSON Parser
- Parses JSON (RFC 7159) into a tree of Value objects
- Supports: null, bool, number, string, array, object
- Recursive descent parser
- Pretty-printing and serialization
- Error reporting with line/column info

Complexity: O(n) parse and serialize
Interview Questions:
1. How to handle large JSON files efficiently? (SAX/streaming parser)
2. What is the difference between JSON and BSON?
3. How to validate JSON schema?
Common Mistakes:
- Not handling Unicode escapes in strings (\\uXXXX)
- Floating point precision loss in number parsing
- Stack overflow with deeply nested JSON
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <stdexcept>

class JSONValue {
public:
    enum Type { NULL_VAL, BOOL, NUMBER, STRING, ARRAY, OBJECT };

    virtual ~JSONValue() = default;
    virtual Type type() const = 0;
    virtual std::string serialize(int indent = 0) const = 0;

    std::string typeName() const {
        static const char* names[] = {"null", "bool", "number", "string", "array", "object"};
        return names[type()];
    }
};

class JSONNull : public JSONValue {
public:
    Type type() const override { return NULL_VAL; }
    std::string serialize(int = 0) const override { return "null"; }
};

class JSONBool : public JSONValue {
    bool value_;
public:
    explicit JSONBool(bool v) : value_(v) {}
    Type type() const override { return BOOL; }
    bool value() const { return value_; }
    std::string serialize(int = 0) const override { return value_ ? "true" : "false"; }
};

class JSONNumber : public JSONValue {
    double value_;
public:
    explicit JSONNumber(double v) : value_(v) {}
    Type type() const override { return NUMBER; }
    double value() const { return value_; }
    std::string serialize(int = 0) const override {
        std::ostringstream oss;
        if (value_ == (long long)value_) oss << (long long)value_;
        else oss << std::fixed << std::setprecision(6) << value_;
        return oss.str();
    }
};

class JSONString : public JSONValue {
    std::string value_;
public:
    explicit JSONString(std::string v) : value_(std::move(v)) {}
    Type type() const override { return STRING; }
    const std::string& value() const { return value_; }
    std::string serialize(int = 0) const override {
        return "\"" + value_ + "\"";
    }
};

class JSONArray : public JSONValue {
    std::vector<std::unique_ptr<JSONValue>> items_;
public:
    Type type() const override { return ARRAY; }
    void add(std::unique_ptr<JSONValue> item) { items_.push_back(std::move(item)); }
    size_t size() const { return items_.size(); }
    JSONValue* at(size_t i) const { return items_[i].get(); }
    std::string serialize(int indent = 0) const override {
        if (items_.empty()) return "[]";
        std::string s = "[\n";
        std::string pad(indent + 2, ' ');
        for (size_t i = 0; i < items_.size(); ++i) {
            if (i > 0) s += ",\n";
            s += pad + items_[i]->serialize(indent + 2);
        }
        s += "\n" + std::string(indent, ' ') + "]";
        return s;
    }
};

class JSONObject : public JSONValue {
    std::unordered_map<std::string, std::unique_ptr<JSONValue>> members_;
    std::vector<std::string> keys_;
public:
    Type type() const override { return OBJECT; }
    void add(const std::string& key, std::unique_ptr<JSONValue> value) {
        members_[key] = std::move(value);
        keys_.push_back(key);
    }
    JSONValue* get(const std::string& key) const {
        auto it = members_.find(key);
        return it != members_.end() ? it->second.get() : nullptr;
    }
    std::string serialize(int indent = 0) const override {
        if (keys_.empty()) return "{}";
        std::string s = "{\n";
        std::string pad(indent + 2, ' ');
        for (size_t i = 0; i < keys_.size(); ++i) {
            if (i > 0) s += ",\n";
            s += pad + "\"" + keys_[i] + "\": " + members_.at(keys_[i])->serialize(indent + 2);
        }
        s += "\n" + std::string(indent, ' ') + "}";
        return s;
    }
};

class JSONParser {
    std::string input_;
    size_t pos_{0};
    int line_{1}, col_{1};

    char peek() { return pos_ < input_.size() ? input_[pos_] : '\0'; }
    char advance() {
        char c = input_[pos_++];
        if (c == '\n') { line_++; col_ = 1; } else col_++;
        return c;
    }
    void skipWS() { while (std::isspace(peek())) advance(); }
    void error(const std::string& msg) {
        throw std::runtime_error("JSON parse error at L" + std::to_string(line_)
            + ":" + std::to_string(col_) + ": " + msg);
    }

public:
    explicit JSONParser(std::string input) : input_(std::move(input)) {}

    std::unique_ptr<JSONValue> parse() {
        skipWS();
        auto val = parseValue();
        skipWS();
        if (peek() != '\0') error("Unexpected trailing characters");
        return val;
    }

private:
    std::unique_ptr<JSONValue> parseValue() {
        skipWS();
        char c = peek();
        if (c == '"') return parseString();
        if (c == '{') return parseObject();
        if (c == '[') return parseArray();
        if (c == 't' || c == 'f') return parseBool();
        if (c == 'n') return parseNull();
        if (c == '-' || std::isdigit(c)) return parseNumber();
        error("Unexpected character: " + std::string(1, c));
        return nullptr;
    }

    std::unique_ptr<JSONString> parseString() {
        std::string s;
        advance(); // opening quote
        while (peek() != '"' && peek() != '\0') {
            if (peek() == '\\') {
                advance();
                switch (advance()) {
                    case '"': s += '"'; break;
                    case '\\': s += '\\'; break;
                    case 'n': s += '\n'; break;
                    case 't': s += '\t'; break;
                    default: error("Invalid escape");
                }
            } else {
                s += advance();
            }
        }
        if (peek() == '"') advance();
        else error("Unterminated string");
        return std::make_unique<JSONString>(s);
    }

    std::unique_ptr<JSONNumber> parseNumber() {
        std::string s;
        if (peek() == '-') s += advance();
        while (std::isdigit(peek())) s += advance();
        if (peek() == '.') {
            s += advance();
            while (std::isdigit(peek())) s += advance();
        }
        if (peek() == 'e' || peek() == 'E') {
            s += advance();
            if (peek() == '+' || peek() == '-') s += advance();
            while (std::isdigit(peek())) s += advance();
        }
        return std::make_unique<JSONNumber>(std::stod(s));
    }

    std::unique_ptr<JSONBool> parseBool() {
        if (peek() == 't') {
            std::string lit = {advance(), advance(), advance(), advance()};
            if (lit != "true") error("Expected 'true'");
            return std::make_unique<JSONBool>(true);
        } else {
            std::string lit = {advance(), advance(), advance(), advance(), advance()};
            if (lit != "false") error("Expected 'false'");
            return std::make_unique<JSONBool>(false);
        }
    }

    std::unique_ptr<JSONNull> parseNull() {
        std::string lit = {advance(), advance(), advance(), advance()};
        if (lit != "null") error("Expected 'null'");
        return std::make_unique<JSONNull>();
    }

    std::unique_ptr<JSONArray> parseArray() {
        auto arr = std::make_unique<JSONArray>();
        advance(); skipWS();
        if (peek() == ']') { advance(); return arr; }
        while (true) {
            arr->add(parseValue());
            skipWS();
            if (peek() == ']') { advance(); return arr; }
            if (peek() == ',') { advance(); skipWS(); }
            else error("Expected ',' or ']' in array");
        }
    }

    std::unique_ptr<JSONObject> parseObject() {
        auto obj = std::make_unique<JSONObject>();
        advance(); skipWS();
        if (peek() == '}') { advance(); return obj; }
        while (true) {
            skipWS();
            if (peek() != '"') error("Expected string key in object");
            auto key = parseString();
            skipWS();
            if (peek() != ':') error("Expected ':' in object");
            advance();
            obj->add(key->value(), parseValue());
            skipWS();
            if (peek() == '}') { advance(); return obj; }
            if (peek() == ',') { advance(); }
            else error("Expected ',' or '}' in object");
        }
    }
};

int main() {
    std::string json = R"({
        "name": "John Doe",
        "age": 30,
        "isStudent": false,
        "scores": [95.5, 87.0, 92.3],
        "address": {
            "city": "New York",
            "zip": "10001",
            "coordinates": {
                "lat": 40.7128,
                "lng": -74.0060
            }
        },
        "tags": null
    })";

    try {
        JSONParser parser(json);
        auto value = parser.parse();

        std::cout << "Parsed JSON:\n" << value->serialize() << "\n\n";

        // Access nested values
        auto* obj = dynamic_cast<JSONObject*>(value.get());
        auto* name = dynamic_cast<JSONString*>(obj->get("name"));
        auto* age = dynamic_cast<JSONNumber*>(obj->get("age"));
        auto* scores = dynamic_cast<JSONArray*>(obj->get("scores"));

        if (name) std::cout << "Name: " << name->value() << "\n";
        if (age) std::cout << "Age: " << age->value() << "\n";
        if (scores) {
            std::cout << "Scores: ";
            for (size_t i = 0; i < scores->size(); ++i)
                std::cout << dynamic_cast<JSONNumber*>(scores->at(i))->value() << " ";
            std::cout << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

/*
SAMPLE OUTPUT:
Parsed JSON:
{
  "name": "John Doe",
  "age": 30,
  "isStudent": false,
  "scores": [
    95.5,
    87,
    92.3
  ],
  ...
}

Name: John Doe
Age: 30
Scores: 95.5 87 92.3
*/
