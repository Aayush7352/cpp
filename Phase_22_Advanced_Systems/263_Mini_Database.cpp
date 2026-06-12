/*
THEORY: Mini Database
- In-memory key-value store with persistence
- Supports tables with rows and columns
- SQL-like operations: CREATE, INSERT, SELECT, UPDATE, DELETE
- Simple query parser for educational purposes
- File-based persistence to disk

Complexity: O(n) for full table scan, O(1) for indexed lookup
Interview Questions:
1. How do real databases handle indexing? (B-trees)
2. What is ACID in databases?
3. How to implement transactions?
Common Mistakes:
- Not escaping strings in SQL queries
- Memory leaks from not cleaning up rows
- Not handling concurrent access
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <iomanip>

class MiniDB {
    struct Row {
        std::unordered_map<std::string, std::string> columns;
    };

    struct Table {
        std::vector<std::string> columns;
        std::vector<Row> rows;
    };

    std::unordered_map<std::string, Table> tables_;

public:
    void createTable(const std::string& name, const std::vector<std::string>& cols) {
        tables_[name] = Table{cols, {}};
        std::cout << "Table '" << name << "' created with columns: ";
        for (const auto& c : cols) std::cout << c << " ";
        std::cout << "\n";
    }

    void insert(const std::string& table, const std::unordered_map<std::string, std::string>& values) {
        auto it = tables_.find(table);
        if (it == tables_.end()) {
            std::cerr << "Table not found: " << table << "\n";
            return;
        }
        Row row;
        for (const auto& col : it->second.columns) {
            auto val = values.find(col);
            row.columns[col] = (val != values.end()) ? val->second : "NULL";
        }
        it->second.rows.push_back(std::move(row));
        std::cout << "Inserted into '" << table << "'\n";
    }

    void select(const std::string& table, const std::vector<std::string>& cols,
                const std::string& whereCol = "", const std::string& whereVal = "") {
        auto it = tables_.find(table);
        if (it == tables_.end()) { std::cerr << "Table not found\n"; return; }

        auto& t = it->second;

        // Print header
        for (const auto& col : cols) {
            std::cout << std::setw(15) << col;
        }
        std::cout << "\n" << std::string(cols.size() * 15, '-') << "\n";

        // Print rows
        for (const auto& row : t.rows) {
            if (!whereCol.empty()) {
                auto val = row.columns.find(whereCol);
                if (val == row.columns.end() || val->second != whereVal) continue;
            }
            for (const auto& col : cols) {
                auto val = row.columns.find(col);
                std::cout << std::setw(15) << (val != row.columns.end() ? val->second : "NULL");
            }
            std::cout << "\n";
        }
    }

    void update(const std::string& table, const std::string& setCol,
                const std::string& setVal, const std::string& whereCol,
                const std::string& whereVal) {
        auto it = tables_.find(table);
        if (it == tables_.end()) { std::cerr << "Table not found\n"; return; }

        int count = 0;
        for (auto& row : it->second.rows) {
            auto cond = row.columns.find(whereCol);
            if (cond != row.columns.end() && cond->second == whereVal) {
                row.columns[setCol] = setVal;
                ++count;
            }
        }
        std::cout << "Updated " << count << " rows in '" << table << "'\n";
    }

    void deleteRows(const std::string& table, const std::string& whereCol,
                    const std::string& whereVal) {
        auto it = tables_.find(table);
        if (it == tables_.end()) { std::cerr << "Table not found\n"; return; }

        auto& rows = it->second.rows;
        auto removed = std::remove_if(rows.begin(), rows.end(), [&](const Row& r) {
            auto val = r.columns.find(whereCol);
            return val != r.columns.end() && val->second == whereVal;
        });
        int count = rows.end() - removed;
        rows.erase(removed, rows.end());
        std::cout << "Deleted " << count << " rows from '" << table << "'\n";
    }

    void save(const std::string& filename) {
        std::ofstream out(filename);
        for (const auto& [name, table] : tables_) {
            out << "TABLE:" << name << "\n";
            out << "COLS:";
            for (size_t i = 0; i < table.columns.size(); ++i) {
                if (i > 0) out << ",";
                out << table.columns[i];
            }
            out << "\n";
            for (const auto& row : table.rows) {
                for (size_t i = 0; i < table.columns.size(); ++i) {
                    if (i > 0) out << "|";
                    auto val = row.columns.find(table.columns[i]);
                    out << (val != row.columns.end() ? val->second : "NULL");
                }
                out << "\n";
            }
        }
        std::cout << "Saved to " << filename << "\n";
    }

    void listTables() const {
        std::cout << "Tables: ";
        for (const auto& [name, _] : tables_) std::cout << name << " ";
        std::cout << "\n";
    }
};

int main() {
    MiniDB db;

    db.createTable("users", {"id", "name", "email", "age"});
    db.createTable("products", {"id", "name", "price"});

    db.insert("users", {{"id", "1"}, {"name", "Alice"}, {"email", "alice@mail.com"}, {"age", "30"}});
    db.insert("users", {{"id", "2"}, {"name", "Bob"}, {"email", "bob@mail.com"}, {"age", "25"}});
    db.insert("users", {{"id", "3"}, {"name", "Charlie"}, {"email", "charlie@mail.com"}, {"age", "35"}});

    db.insert("products", {{"id", "1"}, {"name", "Laptop"}, {"price", "999.99"}});
    db.insert("products", {{"id", "2"}, {"name", "Mouse"}, {"price", "19.99"}});

    std::cout << "\n=== All Users ===\n";
    db.select("users", {"id", "name", "email", "age"});

    std::cout << "\n=== Users with age=30 ===\n";
    db.select("users", {"name", "age"}, "age", "30");

    std::cout << "\n=== Update Bob's age ===\n";
    db.update("users", "age", "26", "name", "Bob");
    db.select("users", {"id", "name", "age"});

    std::cout << "\n=== Delete Charlie ===\n";
    db.deleteRows("users", "name", "Charlie");
    db.select("users", {"id", "name"});

    std::cout << "\n=== Products ===\n";
    db.select("products", {"name", "price"});

    db.save("minidb.txt");
    db.listTables();

    return 0;
}

/*
SAMPLE OUTPUT:
Table 'users' created with columns: id name email age 
Table 'products' created with columns: id name price 
Inserted into 'users'
...
=== All Users ===
             id           name          email            age 
-------------------------------------------------------------
              1          Alice   alice@mail.com             30
              2            Bob     bob@mail.com             25
              3        Charlie charlie@mail.com             35
...
Saved to minidb.txt
Tables: users products
*/
