#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

// --- Observer Pattern ---
class Subscriber {
public:
    virtual void update(const std::string& message) = 0;
    virtual ~Subscriber() = default;
};

class User : public Subscriber {
    std::string name;

public:
    explicit User(const std::string& name) : name(name) {}

    void update(const std::string& message) override {
        std::cout << name << " received notification: " << message << std::endl;
    }
};

// --- Factory Pattern ---
class Notification {
public:
    virtual void send(const std::string& message) = 0;
    virtual ~Notification() = default;
};

class EmailNotification : public Notification {
public:
    void send(const std::string& message) override {
        std::cout << "Sending Email: " << message << std::endl;
    }
};

class SMSNotification : public Notification {
public:
    void send(const std::string& message) override {
        std::cout << "Sending SMS: " << message << std::endl;
    }
};

class PushNotification : public Notification {
public:
    void send(const std::string& message) override {
        std::cout << "Sending Push Notification: " << message << std::endl;
    }
};

class NotificationFactory {
public:
    static std::unique_ptr<Notification> createNotification(const std::string& type) {
        if (type == "email") {
            return std::make_unique<EmailNotification>();
        } else if (type == "sms") {
            return std::make_unique<SMSNotification>();
        } else if (type == "push") {
            return std::make_unique<PushNotification>();
        } else {
            return nullptr;
        }
    }
};

// --- Singleton Pattern ---
class NotificationManager {
private:
    static NotificationManager* instance;
    std::map<std::string, std::vector<Subscriber*>> subscribers;

    NotificationManager() = default;

public:
    static NotificationManager* getInstance() {
        if (!instance) {
            instance = new NotificationManager();
        }
        return instance;
    }

    void subscribe(const std::string& notificationType, Subscriber* subscriber) {
        subscribers[notificationType].push_back(subscriber);
    }

    void notifySubscribers(const std::string& notificationType, const std::string& message) {
        if (subscribers.find(notificationType) != subscribers.end()) {
            for (auto* subscriber : subscribers[notificationType]) {
                subscriber->update(message);
            }
        }
    }

    void sendNotification(const std::string& notificationType, const std::string& message) {
        auto notification = NotificationFactory::createNotification(notificationType);
        if (notification) {
            notification->send(message);
            notifySubscribers(notificationType, message);
        } else {
            std::cout << "Invalid notification type: " << notificationType << std::endl;
        }
    }
};

// Initialize static instance
NotificationManager* NotificationManager::instance = nullptr;

// --- Main Program ---
int main() {
    // Get the NotificationManager instance
    NotificationManager* manager = NotificationManager::getInstance();

    // Create subscribers
    User alice("Alice");
    User bob("Bob");

    // Subscribe users to notifications
    manager->subscribe("email", &alice);
    manager->subscribe("sms", &bob);
    manager->subscribe("push", &alice);
    manager->subscribe("push", &bob);

    // Send notifications
    manager->sendNotification("email", "Your order has been placed.");
    manager->sendNotification("sms", "Your order is on the way.");
    manager->sendNotification("push", "Your order has been delivered.");

    return 0;
}
