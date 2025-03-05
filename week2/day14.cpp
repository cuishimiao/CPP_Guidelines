#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

struct Contact {
    std::string name;
    std::string phone;
    std::string email;
};

class ContactManager {
public:
    void addContact(const Contact& contact) {
        contacts.push_back(contact);
    }

    void deleteContact(const std::string& name) {
        contacts.erase(std::remove_if(contacts.begin(), contacts.end(),
            [&name](const Contact& contact) { return contact.name == name; }), contacts.end());
    }

    void updateContact(const std::string& name, const Contact& newContact) {
        auto it = std::find_if(contacts.begin(), contacts.end(),
            [&name](const Contact& contact) { return contact.name == name; });
        if (it != contacts.end()) {
            *it = newContact;
        }
    }

    void displayContacts() const {
        for (const auto& contact : contacts) {
            std::cout << "Name: " << contact.name << ", Phone: " << contact.phone << ", Email: " << contact.email << std::endl;
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        for (const auto& contact : contacts) {
            file << contact.name << "," << contact.phone << "," << contact.email << std::endl;
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        contacts.clear();
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name, phone, email;
            if (std::getline(iss, name, ',') && std::getline(iss, phone, ',') && std::getline(iss, email, ',')) {
                contacts.push_back({name, phone, email});
            }
        }
    }

private:
    std::vector<Contact> contacts;
};

int main() {
    ContactManager manager;
    manager.addContact({"John Doe", "123-456-7890", "john@example.com"});
    manager.addContact({"Jane Smith", "987-654-3210", "jane@example.com"});
    manager.displayContacts();

    manager.updateContact("John Doe", {"John Doe", "111-222-3333", "john.doe@example.com"});
    manager.displayContacts();

    manager.deleteContact("Jane Smith");
    manager.displayContacts();

    manager.saveToFile("contacts.txt");
    manager.loadFromFile("contacts.txt");
    manager.displayContacts();

    return 0;
}