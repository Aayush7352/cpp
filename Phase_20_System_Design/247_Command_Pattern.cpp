/*
THEORY: Command Pattern
- Behavioral pattern that encapsulates a request as an object
- Parameterizes objects with operations
- Supports undo/redo, queuing, and logging
- Decouples invoker from receiver
- Macro commands: composite of multiple commands

Complexity: O(1) execute/undo
Interview Questions:
1. How to implement undo/redo with unlimited history?
2. Command vs Strategy pattern difference?
3. How to handle command validation?
Common Mistakes:
- Commands holding heavy state causing memory issues
- Not handling exceptions in command execution
- Circular references in macro commands
*/

#include <iostream>
#include <vector>
#include <memory>
#include <stack>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string description() const = 0;
};

class TextEditor {
    std::string text_;
public:
    void insert(const std::string& s, size_t pos) {
        if (pos > text_.size()) pos = text_.size();
        text_.insert(pos, s);
    }
    void erase(size_t pos, size_t len) {
        if (pos + len > text_.size()) len = text_.size() - pos;
        text_.erase(pos, len);
    }
    const std::string& text() const { return text_; }
};

class InsertCommand : public Command {
    TextEditor& editor_;
    std::string text_;
    size_t position_;
public:
    InsertCommand(TextEditor& editor, std::string text, size_t pos)
        : editor_(editor), text_(std::move(text)), position_(pos) {}

    void execute() override { editor_.insert(text_, position_); }
    void undo() override { editor_.erase(position_, text_.size()); }
    std::string description() const override {
        return "Insert '" + text_ + "' at " + std::to_string(position_);
    }
};

class DeleteCommand : public Command {
    TextEditor& editor_;
    std::string deletedText_;
    size_t position_;
public:
    DeleteCommand(TextEditor& editor, size_t pos, size_t len)
        : editor_(editor), position_(pos) {
        if (pos + len <= editor_.text().size())
            deletedText_ = editor_.text().substr(pos, len);
    }

    void execute() override { editor_.erase(position_, deletedText_.size()); }
    void undo() override { editor_.insert(deletedText_, position_); }
    std::string description() const override {
        return "Delete '" + deletedText_ + "' at " + std::to_string(position_);
    }
};

class CommandHistory {
    std::stack<std::unique_ptr<Command>> undoStack_;
    std::stack<std::unique_ptr<Command>> redoStack_;
public:
    void execute(std::unique_ptr<Command> cmd) {
        std::cout << "Executing: " << cmd->description() << "\n";
        cmd->execute();
        undoStack_.push(std::move(cmd));
        while (!redoStack_.empty()) redoStack_.pop();
    }

    void undo() {
        if (undoStack_.empty()) {
            std::cout << "Nothing to undo\n";
            return;
        }
        auto cmd = std::move(undoStack_.top());
        undoStack_.pop();
        std::cout << "Undo: " << cmd->description() << "\n";
        cmd->undo();
        redoStack_.push(std::move(cmd));
    }

    void redo() {
        if (redoStack_.empty()) {
            std::cout << "Nothing to redo\n";
            return;
        }
        auto cmd = std::move(redoStack_.top());
        redoStack_.pop();
        std::cout << "Redo: " << cmd->description() << "\n";
        cmd->execute();
        undoStack_.push(std::move(cmd));
    }
};

int main() {
    TextEditor editor;
    CommandHistory history;

    history.execute(std::make_unique<InsertCommand>(editor, "Hello", 0));
    std::cout << "Text: \"" << editor.text() << "\"\n";

    history.execute(std::make_unique<InsertCommand>(editor, " World", 5));
    std::cout << "Text: \"" << editor.text() << "\"\n";

    history.execute(std::make_unique<DeleteCommand>(editor, 5, 6));
    std::cout << "Text: \"" << editor.text() << "\"\n";

    history.undo();
    std::cout << "Text: \"" << editor.text() << "\"\n";

    history.undo();
    std::cout << "Text: \"" << editor.text() << "\"\n";

    history.redo();
    std::cout << "Text: \"" << editor.text() << "\"\n";

    return 0;
}

/*
SAMPLE OUTPUT:
Executing: Insert 'Hello' at 0
Text: "Hello"
Executing: Insert ' World' at 5
Text: "Hello World"
Executing: Delete ' World' at 5
Text: "Hello"
Undo: Delete ' World' at 5
Text: "Hello World"
Undo: Insert ' World' at 5
Text: "Hello"
Redo: Insert ' World' at 5
Text: "Hello World"
*/
