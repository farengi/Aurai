#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <functional>

// Include all necessary header files
#include "include/models/User.h"
#include "include/models/Admin.h"
#include "include/models/Tutor.h"
#include "include/models/Client.h"
#include "include/models/AIModel.h"
#include "include/models/TutoringSession.h"
#include "include/models/LearningMaterial.h"

#include "include/services/AuthService.h"
#include "include/services/ClientService.h"
#include "include/services/TutorService.h"
#include "include/services/SessionService.h"
#include "include/services/AIModelService.h"
#include "include/services/LearningMaterialService.h"
#include "include/services/ReportService.h"
#include "include/services/FileService.h"

#include "include/utils/Exception.h"
#include "include/utils/Validation.h"
#include "include/utils/Menu.h"
#include "include/utils/Logger.h"

// Function prototypes
void initializeServices();
void createMainMenu();
void handleLogin();
void handleLogout();
void displayAdminMenu();
void displayTutorMenu();
void displayClientManagement();
void displayTutorManagement();
void displaySessionManagement();
void displayAIModelManagement();
void displayMaterialManagement();
void displayReportManagement();

// Global service instances
std::shared_ptr<AuthService> authService;
std::shared_ptr<ClientService> clientService;
std::shared_ptr<TutorService> tutorService;
std::shared_ptr<SessionService> sessionService;
std::shared_ptr<AIModelService> aiModelService;
std::shared_ptr<LearningMaterialService> materialService;
std::shared_ptr<ReportService> reportService;
MenuManager menuManager;

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "     AI Tutoring Center CRM System" << std::endl;
    std::cout << "     Lebanese American University" << std::endl;
    std::cout << "==================================================" << std::endl;
    
    try {
        // Initialize services
        initializeServices();
        
        // Create main menu structure
        createMainMenu();
        
        // Start with login menu
        menuManager.getMenu(0).run(); // Main menu
    }
    catch (const CRMException& e) {
        std::cerr << "Application Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Thank you for using the AI Tutoring Center CRM System!" << std::endl;
    return 0;
}

// In main.cpp, find the initializeServices() function and modify it to:

void initializeServices() {
    // Create service instances for implemented services
    authService = std::make_shared<AuthService>();
    clientService = std::make_shared<ClientService>();
    aiModelService = std::make_shared<AIModelService>();
    
    // Comment out unimplemented services
    /*
    tutorService = std::make_shared<TutorService>();
    sessionService = std::make_shared<SessionService>();
    materialService = std::make_shared<LearningMaterialService>();
    
    // ReportService needs references to other services
    reportService = std::make_shared<ReportService>(
        clientService, tutorService, sessionService, 
        aiModelService, materialService
    );
    */
    
    // Log system startup
    Logger::getInstance().info("CRM System started");
}

// Create the menu structure
void createMainMenu() {
    // Create all menus
    int mainMenuIndex = menuManager.addMenu("AI Tutoring Center CRM System", false);
    int loginMenuIndex = menuManager.addMenu("Login", true);
    int adminMenuIndex = menuManager.addMenu("Administrator Menu", false);
    int tutorMenuIndex = menuManager.addMenu("Tutor Menu", false);
    int clientMenuIndex = menuManager.addMenu("Client Management", false);
    int tutorMgmtMenuIndex = menuManager.addMenu("Tutor Management", false);
    int sessionMenuIndex = menuManager.addMenu("Session Management", false);
    int aiModelMenuIndex = menuManager.addMenu("AI Model Management", false);
    int materialMenuIndex = menuManager.addMenu("Learning Material Management", false);
    int reportMenuIndex = menuManager.addMenu("Reports", false);
    
    // Main menu
    menuManager.getMenu(mainMenuIndex).addItem("Login", [loginMenuIndex]() {
        menuManager.getMenu(loginMenuIndex).run();
    });
    
    menuManager.getMenu(mainMenuIndex).addItem("Exit", []() {
        std::cout << "Exiting system..." << std::endl;
        exit(0);
    });
    
    // Login menu
    menuManager.getMenu(loginMenuIndex).addItem("Enter Credentials", []() {
        handleLogin();
    });
    
    // Admin menu
    menuManager.addSubmenuItem(adminMenuIndex, "Client Management", clientMenuIndex);
    menuManager.addSubmenuItem(adminMenuIndex, "Tutor Management", tutorMgmtMenuIndex);
    menuManager.addSubmenuItem(adminMenuIndex, "Session Management", sessionMenuIndex);
    menuManager.addSubmenuItem(adminMenuIndex, "AI Model Management", aiModelMenuIndex);
    menuManager.addSubmenuItem(adminMenuIndex, "Learning Material Management", materialMenuIndex);
    menuManager.addSubmenuItem(adminMenuIndex, "Reports", reportMenuIndex);
    menuManager.getMenu(adminMenuIndex).addItem("Logout", []() {
        handleLogout();
    });
    
    // Tutor menu
    menuManager.addSubmenuItem(tutorMenuIndex, "View My Schedule", sessionMenuIndex);
    menuManager.addSubmenuItem(tutorMenuIndex, "Manage Sessions", sessionMenuIndex);
    menuManager.addSubmenuItem(tutorMenuIndex, "View Client Information", clientMenuIndex);
    menuManager.addSubmenuItem(tutorMenuIndex, "View AI Models", aiModelMenuIndex);
    menuManager.addSubmenuItem(tutorMenuIndex, "Access Learning Materials", materialMenuIndex);
    menuManager.getMenu(tutorMenuIndex).addItem("Logout", []() {
        handleLogout();
    });
    
    // Client Management Menu
    menuManager.getMenu(clientMenuIndex).addItem("Add New Client", []() {
        std::cout << "Add New Client functionality to be implemented" << std::endl;
    });
    
    menuManager.getMenu(clientMenuIndex).addItem("Search Clients", []() {
        std::cout << "Search Clients functionality to be implemented" << std::endl;
    });
    
    menuManager.getMenu(clientMenuIndex).addItem("View Client Details", []() {
        std::cout << "View Client Details functionality to be implemented" << std::endl;
    });
    
    menuManager.getMenu(clientMenuIndex).addItem("Update Client Information", []() {
        std::cout << "Update Client Information functionality to be implemented" << std::endl;
    });
    
    menuManager.getMenu(clientMenuIndex).addItem("Delete Client", []() {
        std::cout << "Delete Client functionality to be implemented" << std::endl;
    });
    
    // Similarly, populate other menus with appropriate items
    // (Tutor Management, Session Management, etc.)
    // This would be expanded in the full implementation
}

// Handle login process
void handleLogin() {
    std::string username, password;
    
    std::cout << "Username: ";
    std::getline(std::cin, username);
    
    std::cout << "Password: ";
    std::getline(std::cin, password);
    
    try {
        bool success = authService->login(username, password);
        
        if (success) {
            Logger::getInstance().info("User logged in: " + username);
            
            if (authService->isAdmin()) {
                std::cout << "Admin login successful!" << std::endl;
                menuManager.getMenu(2).run(); // Admin menu
            } else if (authService->isTutor()) {
                std::cout << "Tutor login successful!" << std::endl;
                menuManager.getMenu(3).run(); // Tutor menu
            }
        } else {
            std::cout << "Login failed. Invalid username or password." << std::endl;
            Logger::getInstance().warning("Failed login attempt: " + username);
        }
    }
    catch (const CRMException& e) {
        std::cout << "Login error: " << e.what() << std::endl;
        Logger::getInstance().error("Login error: " + std::string(e.what()));
    }
}

// Handle logout process
void handleLogout() {
    if (authService->isUserLoggedIn()) {
        std::string username = authService->getCurrentUser()->getUsername();
        authService->logout();
        std::cout << "Logged out successfully." << std::endl;
        Logger::getInstance().info("User logged out: " + username);
    }
}

// The implementations for other menu handler functions would follow
// displayAdminMenu(), displayTutorMenu(), etc.