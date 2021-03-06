#include <memory>

#include "packs/BaseApp/App3D.h"

#include "event_system/Dispatcher.h"
#include "event_system/Subscriber.h"

class BaseApp {
  public:
    class BaseAppStaticInit {
        std::shared_ptr<App3D> app;
        Subscriber* init_subscriber;

      public:
        BaseAppStaticInit() {
            init_subscriber = new Subscriber(this, Function_Cast(&BaseAppStaticInit::init), false);
            Dispatcher::GetInstance()->AddEventSubscriber(init_subscriber, "EVENT_RENDER_INIT_SUCCESS");
        }

        ~BaseAppStaticInit() { delete init_subscriber; }

        void init(std::shared_ptr<void> event_data) {
            std::cout << "BaseApp::Init " << event_data << std::endl;

            app = std::shared_ptr<App3D>(new App3D());

            std::shared_ptr<Renderer> renderer = std::static_pointer_cast<Renderer>(event_data);
            if (renderer.get() == nullptr) {
                std::cout << "Initialization Order Error: The Application tried to start without a renderer."
                          << std::endl;
                exit(1);
            }

            if (!app->initialize(std::static_pointer_cast<Renderer>(event_data))) {
                printf("App could not Initialize!");
                exit(1);
            }

            Dispatcher::GetInstance()->DispatchImmediate("EVENT_APP_INIT_SUCCESS", app);
        }
    };

    friend class BaseAppStaticInit;

  public:
    static BaseApp::BaseAppStaticInit init;
};

BaseApp::BaseAppStaticInit BaseApp::init;
static BaseApp BASE_APP_NORMAL_NAME;
