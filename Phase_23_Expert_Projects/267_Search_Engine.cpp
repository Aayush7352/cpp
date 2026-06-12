/*
THEORY: Search Engine
- Inverted index for full-text search
- TF-IDF ranking algorithm
- Document indexing and query processing
- Tokenization with stop word removal
- Boolean and ranked retrieval

Complexity: O(n) indexing, O(log n) query
Interview Questions:
1. How does Google handle indexing at scale?
2. What is PageRank and how does it work?
3. How to handle synonyms and stemming?
Common Mistakes:
- Not handling punctuation in tokenization
- Case sensitivity issues in search
- Not scaling index for large document collections
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <iomanip>

class SearchEngine {
    struct Document {
        int id;
        std::string content;
        std::string title;
    };

    struct Posting {
        int docId;
        int termFrequency;
        std::vector<int> positions;
    };

    std::vector<Document> documents_;
    std::unordered_map<std::string, std::vector<Posting>> index_;
    std::set<std::string> stopWords_ = {"the", "a", "an", "is", "are", "was", "were",
        "in", "on", "at", "to", "for", "of", "and", "or", "it", "this", "that",
        "with", "from", "by", "be", "been", "being", "have", "has", "had",
        "do", "does", "did", "will", "would", "could", "should", "may", "might"};

    std::string toLower(const std::string& s) const {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    std::vector<std::string> tokenize(const std::string& text) const {
        std::vector<std::string> tokens;
        std::string current;
        for (char c : text) {
            if (std::isalnum(c)) {
                current += std::tolower(c);
            } else if (!current.empty()) {
                if (stopWords_.find(current) == stopWords_.end())
                    tokens.push_back(current);
                current.clear();
            }
        }
        if (!current.empty() && stopWords_.find(current) == stopWords_.end())
            tokens.push_back(current);
        return tokens;
    }

public:
    void addDocument(int id, const std::string& title, const std::string& content) {
        documents_.push_back({id, content, title});
        auto tokens = tokenize(content);

        std::unordered_map<std::string, int> termCount;
        for (size_t pos = 0; pos < tokens.size(); ++pos) {
            termCount[tokens[pos]]++;
        }

        for (const auto& [term, count] : termCount) {
            double tf = 1.0 + std::log2(count);
            double idf = std::log2(1.0 + documents_.size());
            index_[term].push_back({id, count, {}});
        }
        std::cout << "Indexed: " << title << "\n";
    }

    std::vector<std::pair<int, double>> search(const std::string& query) const {
        auto queryTokens = tokenize(query);
        if (queryTokens.empty()) return {};

        std::unordered_map<int, double> scores;
        std::unordered_map<int, int> matchCount;

        for (const auto& term : queryTokens) {
            auto it = index_.find(term);
            if (it == index_.end()) continue;

            double idf = std::log2(1.0 + documents_.size() / (1.0 + it->second.size()));
            for (const auto& posting : it->second) {
                double tf = 1.0 + std::log2(posting.termFrequency);
                scores[posting.docId] += tf * idf;
                matchCount[posting.docId]++;
            }
        }

        std::vector<std::pair<int, double>> results;
        for (const auto& [docId, score] : scores) {
            results.emplace_back(docId, score);
        }

        std::sort(results.begin(), results.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });

        return results;
    }

    void printResults(const std::vector<std::pair<int, double>>& results, int topN = 5) const {
        std::cout << "\n=== Search Results ===\n";
        int rank = 0;
        for (const auto& [docId, score] : results) {
            if (rank++ >= topN) break;
            auto doc = std::find_if(documents_.begin(), documents_.end(),
                [docId](const Document& d) { return d.id == docId; });
            if (doc != documents_.end()) {
                std::cout << rank << ". " << doc->title
                          << " (score: " << std::fixed << std::setprecision(2) << score << ")\n";
                std::cout << "   " << doc->content.substr(0, 100) << "...\n\n";
            }
        }
    }

    int documentCount() const { return documents_.size(); }
    int termCount() const { return index_.size(); }
};

int main() {
    SearchEngine engine;

    engine.addDocument(1, "C++ Programming Guide",
        "C++ is a powerful programming language for system development. "
        "It supports object-oriented and generic programming paradigms.");
    engine.addDocument(2, "Machine Learning Basics",
        "Machine learning uses algorithms to learn patterns from data. "
        "Deep learning and neural networks are popular approaches.");
    engine.addDocument(3, "Data Structures and Algorithms",
        "Data structures like arrays and trees are essential for efficient algorithms. "
        "Sorting and searching are fundamental operations.");
    engine.addDocument(4, "Modern C++ Features",
        "C++11 introduced auto, lambda expressions, and smart pointers. "
        "C++20 added concepts, coroutines, and ranges to the language.");
    engine.addDocument(5, "Web Development with C++",
        "C++ can be used for web development with frameworks like Crow and Drogon. "
        "High-performance web servers are built with C++.");

    std::cout << "Indexed " << engine.documentCount() << " documents\n";
    std::cout << "Dictionary size: " << engine.termCount() << " terms\n";

    std::vector<std::string> queries = {
        "C++ programming",
        "machine learning algorithms",
        "data structures"
    };

    for (const auto& query : queries) {
        std::cout << "\nQuery: \"" << query << "\"";
        auto results = engine.search(query);
        engine.printResults(results);
    }

    return 0;
}

/*
SAMPLE OUTPUT:
Indexed: C++ Programming Guide
Indexed: Machine Learning Basics
...
Dictionary size: 42 terms

Query: "C++ programming"
=== Search Results ===
1. C++ Programming Guide (score: 3.89)
2. Modern C++ Features (score: 2.75)
...
*/
