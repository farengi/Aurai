#include "include/services/ClientService.h"
#include "include/utils/Logger.h"
#include "include/utils/Exception.h"
#include "include/utils/Validation.h"
#include <algorithm>
#include <iostream>

// Constructor
ClientService::ClientService() : nextClientId(1) {
    loadClients();
}

// Destructor
ClientService::~ClientService() {
    saveClients();
}

// Load clients from file (stub implementation)
void ClientService::loadClients() {
    // In a real implementation, this would load from a file
    // For now, we'll create some sample clients for testing
    
    auto client1 = std::make_shared<Client>(
        1, "John", "Doe", 
        "john.doe@example.com", "555-1111", 
        "TechCorp", "CTO"
    );
    client1->addAIModel("GPT-4");
    client1->addAIModel("DALL-E 3");
    client1->setAIModelProficiency("GPT-4", 3);
    client1->setRegistrationDate("2025-01-15");
    client1->setSessionsCompleted(5);
    client1->setBudget(1000.0);
    
    auto client2 = std::make_shared<Client>(
        2, "Jane", "Smith", 
        "jane.smith@example.com", "555-2222", 
        "DataAnalytics Inc", "Data Scientist"
    );
    client2->addAIModel("GPT-4");
    client2->addAIModel("Stable Diffusion");
    client2->addAIModel("BERT");
    client2->setAIModelProficiency("BERT", 4);
    client2->setRegistrationDate("2025-02-10");
    client2->setSessionsCompleted(3);
    client2->setBudget(750.0);
    
    clients.push_back(client1);
    clients.push_back(client2);
    
    nextClientId = 3; // Set next ID after creating sample clients
    
    Logger::getInstance().info("Loaded " + std::to_string(clients.size()) + " clients");
}

// Save clients to file (stub implementation)
void ClientService::saveClients() {
    // In a real implementation, this would save to a file
    Logger::getInstance().info("Saved " + std::to_string(clients.size()) + " clients");
}

// Validation methods
bool ClientService::isValidEmail(const std::string& email) const {
    return Validation::isValidEmail(email);
}

bool ClientService::isValidPhone(const std::string& phone) const {
    return Validation::isValidPhone(phone);
}

// Client management methods
bool ClientService::addClient(std::shared_ptr<Client> client) {
    // Validate client data
    if (!isValidEmail(client->getEmail())) {
        throw ValidationException("Invalid email address");
    }
    
    if (!isValidPhone(client->getPhone())) {
        throw ValidationException("Invalid phone number");
    }
    
    // Set client ID if not already set
    if (client->getClientId() == 0) {
        client->setClientId(nextClientId++);
    }
    
    clients.push_back(client);
    saveClients();
    
    Logger::getInstance().info("Added new client: " + client->getFullName());
    return true;
}

bool ClientService::removeClient(int clientId) {
    auto it = std::find_if(clients.begin(), clients.end(), 
        [clientId](const std::shared_ptr<Client>& client) {
            return client->getClientId() == clientId;
        });
    
    if (it != clients.end()) {
        Logger::getInstance().info("Removed client: " + (*it)->getFullName());
        clients.erase(it);
        saveClients();
        return true;
    }
    
    return false;
}

bool ClientService::updateClientDetails(int clientId, const std::string& firstName, 
                                      const std::string& lastName, const std::string& email,
                                      const std::string& phone, const std::string& company,
                                      const std::string& position) {
    auto client = getClientById(clientId);
    if (!client) {
        return false;
    }
    
    // Validate new data
    if (!isValidEmail(email)) {
        throw ValidationException("Invalid email address");
    }
    
    if (!isValidPhone(phone)) {
        throw ValidationException("Invalid phone number");
    }
    
    client->setFirstName(firstName);
    client->setLastName(lastName);
    client->setEmail(email);
    client->setPhone(phone);
    client->setCompany(company);
    client->setPosition(position);
    
    saveClients();
    Logger::getInstance().info("Updated client details: " + client->getFullName());
    return true;
}

// Client retrieval methods
std::shared_ptr<Client> ClientService::getClientById(int clientId) const {
    for (const auto& client : clients) {
        if (client->getClientId() == clientId) {
            return client;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Client>> ClientService::getAllClients() const {
    return clients;
}

std::vector<std::shared_ptr<Client>> ClientService::searchClientsByName(const std::string& name) const {
    std::vector<std::shared_ptr<Client>> results;
    
    std::string searchName = name;
    // Convert search name to lowercase for case-insensitive search
    std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);
    
    for (const auto& client : clients) {
        std::string fullName = client->getFullName();
        std::transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
        
        if (fullName.find(searchName) != std::string::npos) {
            results.push_back(client);
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Client>> ClientService::searchClientsByCompany(const std::string& company) const {
    std::vector<std::shared_ptr<Client>> results;
    
    std::string searchCompany = company;
    // Convert search company to lowercase for case-insensitive search
    std::transform(searchCompany.begin(), searchCompany.end(), searchCompany.begin(), ::tolower);
    
    for (const auto& client : clients) {
        std::string clientCompany = client->getCompany();
        std::transform(clientCompany.begin(), clientCompany.end(), clientCompany.begin(), ::tolower);
        
        if (clientCompany.find(searchCompany) != std::string::npos) {
            results.push_back(client);
        }
    }
    
    return results;
}

std::vector<std::shared_ptr<Client>> ClientService::getClientsInterestedInModel(const std::string& aiModel) const {
    std::vector<std::shared_ptr<Client>> results;
    
    for (const auto& client : clients) {
        if (client->isInterestedIn(aiModel)) {
            results.push_back(client);
        }
    }
    
    return results;
}

// Client progress tracking methods
bool ClientService::updateClientProgress(int clientId, const std::string& aiModel, int proficiencyLevel) {
    auto client = getClientById(clientId);
    if (!client) {
        return false;
    }
    
    client->setAIModelProficiency(aiModel, proficiencyLevel);
    saveClients();
    
    Logger::getInstance().info("Updated progress for client " + client->getFullName() + 
                              " on " + aiModel + " to level " + std::to_string(proficiencyLevel));
    return true;
}

std::map<std::string, int> ClientService::getClientProficiencies(int clientId) const {
    auto client = getClientById(clientId);
    if (!client) {
        return {};
    }
    
    return client->getAllProficiencies();
}

// Client AI model management
bool ClientService::addClientInterest(int clientId, const std::string& aiModel) {
    auto client = getClientById(clientId);
    if (!client) {
        return false;
    }
    
    client->addAIModel(aiModel);
    saveClients();
    
    Logger::getInstance().info("Added interest in " + aiModel + " for client " + client->getFullName());
    return true;
}

bool ClientService::removeClientInterest(int clientId, const std::string& aiModel) {
    auto client = getClientById(clientId);
    if (!client) {
        return false;
    }
    
    client->removeAIModel(aiModel);
    saveClients();
    
    Logger::getInstance().info("Removed interest in " + aiModel + " for client " + client->getFullName());
    return true;
}

// Client session management
bool ClientService::updateClientSessionInfo(int clientId, const std::string& sessionDate) {
    auto client = getClientById(clientId);
    if (!client) {
        return false;
    }
    
    client->incrementSessionsCompleted();
    client->setLastSessionDate(sessionDate);
    saveClients();
    
    Logger::getInstance().info("Updated session info for client " + client->getFullName() + 
                              ", total sessions: " + std::to_string(client->getSessionsCompleted()));
    return true;
}

int ClientService::getClientCompletedSessions(int clientId) const {
    auto client = getClientById(clientId);
    if (!client) {
        return 0;
    }
    
    return client->getSessionsCompleted();
}

// Client budget management
bool ClientService::updateClientBudget(int clientId, double amount, bool isAddition) {
    auto client = getClientById(clientId);
    if (!client) {
        return false;
    }
    
    if (isAddition) {
        client->addToBudget(amount);
        Logger::getInstance().info("Added $" + std::to_string(amount) + " to budget for client " + 
                                  client->getFullName());
    } else {
        client->deductFromBudget(amount);
        Logger::getInstance().info("Deducted $" + std::to_string(amount) + " from budget for client " + 
                                  client->getFullName());
    }
    
    saveClients();
    return true;
}

double ClientService::getClientBudget(int clientId) const {
    auto client = getClientById(clientId);
    if (!client) {
        return 0.0;
    }
    
    return client->getBudget();
}

// Analytics methods
std::map<std::string, int> ClientService::getPopularAIModels() const {
    std::map<std::string, int> modelCounts;
    
    for (const auto& client : clients) {
        const auto& interests = client->getAIModelsOfInterest();
        for (const auto& model : interests) {
            modelCounts[model]++;
        }
    }
    
    return modelCounts;
}

double ClientService::getAverageClientSessions() const {
    if (clients.empty()) {
        return 0.0;
    }
    
    int totalSessions = 0;
    for (const auto& client : clients) {
        totalSessions += client->getSessionsCompleted();
    }
    
    return static_cast<double>(totalSessions) / clients.size();
}

std::vector<std::shared_ptr<Client>> ClientService::getTopClients(int count) const {
    // Create a copy of clients
    auto sortedClients = clients;
    
    // Sort by number of completed sessions (descending)
    std::sort(sortedClients.begin(), sortedClients.end(),
              [](const std::shared_ptr<Client>& a, const std::shared_ptr<Client>& b) {
                  return a->getSessionsCompleted() > b->getSessionsCompleted();
              });
    
    // Return the requested number (or all if count is larger)
    int resultCount = std::min(static_cast<int>(sortedClients.size()), count);
    return std::vector<std::shared_ptr<Client>>(sortedClients.begin(), sortedClients.begin() + resultCount);
}