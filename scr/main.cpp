#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

class VocabularyTrainer {
private:
    std::string data_path = "./data/vocabulary.csv";
    
    struct Vocabulary {
        std::string german;
        std::string spanish;
    };

    std::vector<Vocabulary> load_vocabulary() {
        std::vector<Vocabulary> vocab;
        std::ifstream file(data_path);
        
        if (!file.is_open()) {
            std::cerr << "Fehler: Vokabeldatei nicht gefunden!\n";
            return vocab;
        }

        std::string line;
        while (std::getline(file, line)) {
            size_t delimiter = line.find(',');
            if (delimiter != std::string::npos) {
                vocab.push_back({
                    line.substr(0, delimiter),
                    line.substr(delimiter + 1)
                });
            }
        }
        return vocab;
    }

    void save_vocabulary(const Vocabulary& vocab) {
        std::ofstream file(data_path, std::ios::app);
        if (file) {
            file << vocab.german << "," << vocab.spanish << "\n";
        }
    }

public:
    void add_vocabulary() {
        Vocabulary new_vocab;
        std::cout << "Deutsche Vokabel: ";
        std::getline(std::cin >> std::ws, new_vocab.german);
        
        std::cout << "Spanische Übersetzung: ";
        std::getline(std::cin >> std::ws, new_vocab.spanish);

        save_vocabulary(new_vocab);
        std::cout << "Vokabel erfolgreich gespeichert!\n\n";
    }

    void start_test() {
        auto vocab = load_vocabulary();
        if (vocab.empty()) {
            std::cout << "Keine Vokabeln vorhanden!\n";
            return;
        }

        std::shuffle(vocab.begin(), vocab.end(), 
                   std::default_random_engine(static_cast<unsigned>(time(0))));

        int correct = 0;
        for (const auto& entry : vocab) {
            std::string answer;
            std::cout << "Übersetze '" << entry.german << "': ";
            std::getline(std::cin >> std::ws, answer);

            if (answer == entry.spanish) {
                std::cout << "Richtig!\n";
                correct++;
            } else {
                std::cout << "Falsch. Richtige Antwort: " << entry.spanish << "\n";
            }
        }

        std::cout << "\nErgebnis: " << correct << "/" << vocab.size()
                << " richtig (" << (correct * 100 / vocab.size()) << "%)\n\n";
    }
};

void show_menu() {
    VocabularyTrainer trainer;
    
    while (true) {
        std::cout << "=== Vokabeltrainer ===\n"
                << "1. Neue Vokabel hinzufügen\n"
                << "2. Vokabeltest starten\n"
                << "3. Beenden\n"
                << "Auswahl: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                trainer.add_vocabulary();
                break;
            case 2:
                trainer.start_test();
                break;
            case 3:
                return;
            default:
                std::cout << "Ungültige Eingabe!\n";
        }
    }
}

int main() {
    show_menu();
    return 0;
}