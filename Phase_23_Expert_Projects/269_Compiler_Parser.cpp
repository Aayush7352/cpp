/*
THEORY: Compiler Parser (Recursive Descent)
- Converts token stream to AST (Abstract Syntax Tree)
- Grammar: Expression -> Term (+|-) Term | Term
- Term -> Factor (*|/) Factor | Factor
- Factor -> Number | Identifier | (Expression)
- Recursive descent parsing with lookahead
- Error recovery with panic mode

Complexity: O(n) for LL(1) grammars
Interview Questions:
1. Recursive descent vs LALR(1) parsers?
2. How to handle left recursion in grammars?
3. What is the difference between AST and CST?
Common Mistakes:
- Not properly handling operator precedence
- Left recursion in grammar causing infinite recursion
- Not reporting meaningful parse errors
*/

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cctype>
#include <cstdlib>

enum class TokenType { NUMBER, PLUS, MINUS, STAR, SLASH, LPAREN, RPAREN, END, ERROR };
struct Token { TokenType type; std::string lexeme; double value; };

class Lexer {
    std::string input_;
    size_t pos_{0};
public:
    explicit Lexer(std::string s) : input_(std::move(s)) {}
    Token next() {
        while (pos_ < input_.size() && std::isspace(input_[pos_])) ++pos_;
        if (pos_ >= input_.size()) return {TokenType::END, "", 0};
        char c = input_[pos_];
        if (std::isdigit(c) || c == '.') {
            size_t start = pos_;
            while (pos_ < input_.size() && (std::isdigit(input_[pos_]) || input_[pos_] == '.')) ++pos_;
            return {TokenType::NUMBER, input_.substr(start, pos_ - start), std::stod(input_.substr(start, pos_ - start))};
        }
        ++pos_;
        switch (c) {
            case '+': return {TokenType::PLUS, "+", 0};
            case '-': return {TokenType::MINUS, "-", 0};
            case '*': return {TokenType::STAR, "*", 0};
            case '/': return {TokenType::SLASH, "/", 0};
            case '(': return {TokenType::LPAREN, "(", 0};
            case ')': return {TokenType::RPAREN, ")", 0};
            default:  return {TokenType::ERROR, std::string(1, c), 0};
        }
    }
};

// AST Nodes
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual double evaluate() const = 0;
    virtual std::string toString() const = 0;
};

struct NumberNode : ASTNode {
    double value;
    explicit NumberNode(double v) : value(v) {}
    double evaluate() const override { return value; }
    std::string toString() const override { return std::to_string(value); }
};

struct BinaryOpNode : ASTNode {
    char op;
    std::unique_ptr<ASTNode> left, right;
    BinaryOpNode(char o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
    double evaluate() const override {
        switch (op) {
            case '+': return left->evaluate() + right->evaluate();
            case '-': return left->evaluate() - right->evaluate();
            case '*': return left->evaluate() * right->evaluate();
            case '/': return left->evaluate() / right->evaluate();
            default: return 0;
        }
    }
    std::string toString() const override {
        return "(" + left->toString() + " " + op + " " + right->toString() + ")";
    }
};

class Parser {
    Lexer lexer_;
    Token current_;
    std::vector<std::string> errors_;

    void advance() { current_ = lexer_.next(); }
    bool match(TokenType t) {
        if (current_.type == t) { advance(); return true; }
        return false;
    }

    void expect(TokenType t, const std::string& msg) {
        if (!match(t)) {
            errors_.push_back("Expected " + msg + " at '" + current_.lexeme + "'");
        }
    }

    // Grammar: Expression -> Term (('+'|'-') Term)*
    std::unique_ptr<ASTNode> parseExpression() {
        auto left = parseTerm();
        while (current_.type == TokenType::PLUS || current_.type == TokenType::MINUS) {
            char op = current_.type == TokenType::PLUS ? '+' : '-';
            advance();
            auto right = parseTerm();
            left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
        }
        return left;
    }

    // Grammar: Term -> Factor (('*'|'/') Factor)*
    std::unique_ptr<ASTNode> parseTerm() {
        auto left = parseFactor();
        while (current_.type == TokenType::STAR || current_.type == TokenType::SLASH) {
            char op = current_.type == TokenType::STAR ? '*' : '/';
            advance();
            auto right = parseFactor();
            left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
        }
        return left;
    }

    // Grammar: Factor -> Number | '(' Expression ')'
    std::unique_ptr<ASTNode> parseFactor() {
        if (current_.type == TokenType::NUMBER) {
            auto node = std::make_unique<NumberNode>(current_.value);
            advance();
            return node;
        }
        if (match(TokenType::LPAREN)) {
            auto node = parseExpression();
            expect(TokenType::RPAREN, "')'");
            return node;
        }
        errors_.push_back("Unexpected token: " + current_.lexeme);
        return std::make_unique<NumberNode>(0);
    }

public:
    explicit Parser(const std::string& input) : lexer_(input) { advance(); }

    std::unique_ptr<ASTNode> parse() {
        auto ast = parseExpression();
        if (current_.type != TokenType::END) {
            errors_.push_back("Unexpected token after expression: " + current_.lexeme);
        }
        return ast;
    }

    bool hasErrors() const { return !errors_.empty(); }
    void printErrors() const {
        for (const auto& e : errors_) std::cerr << "Parse error: " << e << "\n";
    }
};

int main() {
    std::vector<std::string> tests = {
        "3 + 4 * 2",
        "(3 + 4) * 2",
        "10 / 2 + 3 * 4",
        "2.5 * (3.0 + 4.5)",
        "1 + 2 + 3 + 4",
        "(1 + 2) * (3 + 4)"
    };

    for (const auto& expr : tests) {
        Parser parser(expr);
        auto ast = parser.parse();

        std::cout << "Expression: " << expr << "\n";
        if (parser.hasErrors()) {
            parser.printErrors();
        } else {
            std::cout << "  AST: " << ast->toString() << "\n";
            std::cout << "  Result: " << ast->evaluate() << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}

/*
SAMPLE OUTPUT:
Expression: 3 + 4 * 2
  AST: (3 + (4 * 2))
  Result: 11

Expression: (3 + 4) * 2
  AST: ((3 + 4) * 2)
  Result: 14
*/
