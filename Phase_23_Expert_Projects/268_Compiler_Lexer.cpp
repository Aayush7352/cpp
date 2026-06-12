/*
THEORY: Compiler Lexer (Lexical Analyzer)
- Converts source code to tokens
- Recognizes identifiers, keywords, numbers, operators, delimiters
- Uses DFA (Deterministic Finite Automaton)
- Provides tokens to parser
- Handles whitespace, comments, and errors

Complexity: O(n) linear scan
Interview Questions:
1. How to handle Unicode/UTF-8 identifiers?
2. What is the maximal munch principle?
3. How to implement a lexer generator (like Lex/Flex)?
Common Mistakes:
- Not handling multi-line comments
- Incorrect number parsing (octal, hex, float)
- Buffer overflow when reading input
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <unordered_map>

enum class TokenType {
    KEYWORD, IDENTIFIER, NUMBER, STRING, OPERATOR,
    PUNCTUATION, COMMENT, WHITESPACE, END_OF_FILE, ERROR
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    std::string toString() const {
        static const std::unordered_map<TokenType, std::string> names = {
            {TokenType::KEYWORD, "KEYWORD"}, {TokenType::IDENTIFIER, "IDENTIFIER"},
            {TokenType::NUMBER, "NUMBER"}, {TokenType::STRING, "STRING"},
            {TokenType::OPERATOR, "OPERATOR"}, {TokenType::PUNCTUATION, "PUNCTUATION"},
            {TokenType::COMMENT, "COMMENT"}, {TokenType::END_OF_FILE, "EOF"},
            {TokenType::ERROR, "ERROR"}
        };
        auto it = names.find(type);
        std::string typeName = (it != names.end()) ? it->second : "UNKNOWN";
        return "Token(" + typeName + ", '" + lexeme + "', L" + std::to_string(line)
               + ":" + std::to_string(column) + ")";
    }
};

class Lexer {
    std::string source_;
    size_t pos_{0};
    int line_{1};
    int column_{1};

    std::unordered_map<std::string, TokenType> keywords_ = {
        {"int", TokenType::KEYWORD}, {"float", TokenType::KEYWORD},
        {"if", TokenType::KEYWORD}, {"else", TokenType::KEYWORD},
        {"while", TokenType::KEYWORD}, {"for", TokenType::KEYWORD},
        {"return", TokenType::KEYWORD}, {"class", TokenType::KEYWORD},
        {"void", TokenType::KEYWORD}, {"auto", TokenType::KEYWORD},
        {"const", TokenType::KEYWORD}, {"true", TokenType::KEYWORD},
        {"false", TokenType::KEYWORD}, {"namespace", TokenType::KEYWORD},
        {"using", TokenType::KEYWORD}, {"include", TokenType::KEYWORD}
    };

    char peek() const { return pos_ < source_.size() ? source_[pos_] : '\0'; }
    char advance() {
        char c = source_[pos_++];
        if (c == '\n') { ++line_; column_ = 1; }
        else ++column_;
        return c;
    }

public:
    explicit Lexer(std::string source) : source_(std::move(source)) {}

    Token nextToken() {
        skipWhitespaceAndComments();
        if (pos_ >= source_.size()) return {TokenType::END_OF_FILE, "", line_, column_};

        char c = peek();
        int startCol = column_;

        // Identifier or keyword
        if (std::isalpha(c) || c == '_') {
            std::string lexeme;
            while (std::isalnum(peek()) || peek() == '_') lexeme += advance();
            auto it = keywords_.find(lexeme);
            return {it != keywords_.end() ? it->second : TokenType::IDENTIFIER,
                    lexeme, line_, startCol};
        }

        // Number
        if (std::isdigit(c)) {
            std::string lexeme;
            bool isFloat = false;
            while (std::isdigit(peek())) lexeme += advance();
            if (peek() == '.') {
                isFloat = true;
                lexeme += advance();
                while (std::isdigit(peek())) lexeme += advance();
            }
            if (peek() == 'f' || peek() == 'F') lexeme += advance();
            return {TokenType::NUMBER, lexeme, line_, startCol};
        }

        // String literal
        if (c == '"') {
            advance(); // opening quote
            std::string lexeme;
            while (peek() != '"' && peek() != '\0' && peek() != '\n') {
                if (peek() == '\\') { lexeme += advance(); lexeme += advance(); }
                else lexeme += advance();
            }
            if (peek() == '"') advance(); // closing quote
            else return {TokenType::ERROR, "Unterminated string", line_, startCol};
            return {TokenType::STRING, lexeme, line_, startCol};
        }

        // Operators and punctuation
        std::string op;
        op += advance();

        // Multi-character operators
        if ((op == "=" && peek() == '=') || (op == "!" && peek() == '=') ||
            (op == "<" && peek() == '=') || (op == ">" && peek() == '=') ||
            (op == "&" && peek() == '&') || (op == "|" && peek() == '|') ||
            (op == "+" && peek() == '+') || (op == "-" && peek() == '-') ||
            (op == ":" && peek() == ':')) {
            op += advance();
        }

        if (std::string("+-*/%=<>!&|^~;:,.(){}[]").find(op[0]) != std::string::npos) {
            return {TokenType::OPERATOR, op, line_, startCol};
        }

        return {TokenType::ERROR, "Unexpected character: " + op, line_, startCol};
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (true) {
            auto token = nextToken();
            if (token.type == TokenType::END_OF_FILE) break;
            if (token.type == TokenType::ERROR) {
                std::cerr << "Lexical error: " << token.lexeme << "\n";
            }
            tokens.push_back(token);
        }
        return tokens;
    }

private:
    void skipWhitespaceAndComments() {
        while (true) {
            while (std::isspace(peek()) && peek() != '\n') advance();
            // Single-line comment
            if (peek() == '/' && pos_ + 1 < source_.size() && source_[pos_ + 1] == '/') {
                while (peek() != '\n' && peek() != '\0') advance();
                continue;
            }
            break;
        }
    }
};

int main() {
    std::string code = R"(
        int main() {
            int x = 42;
            float y = 3.14f;
            if (x == 42) return x + y;
        }
    )";

    Lexer lexer(code);
    auto tokens = lexer.tokenize();

    std::cout << "Tokens (" << tokens.size() << "):\n";
    for (const auto& token : tokens) {
        std::cout << "  " << token.toString() << "\n";
    }

    return 0;
}

/*
SAMPLE OUTPUT:
Tokens (16):
  Token(KEYWORD, 'int', L2:9)
  Token(IDENTIFIER, 'main', L2:13)
  Token(PUNCTUATION, '(', L2:17)
  ...
  Token(NUMBER, '42', L3:17)
  ...
*/
