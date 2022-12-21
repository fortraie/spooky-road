#ifndef PSWMANAGER_SPOOKY_ROAD_COMMUNICATION_H
#define PSWMANAGER_SPOOKY_ROAD_COMMUNICATION_H

#include "../classes/Session.h"
#include "../classes/Category.h"
#include "../classes/Entry.h"


namespace com {


    void initialize();

    void returning_user();
    void new_user();


    namespace returningUser {


        void password_notification(Session& session, bool status);


        namespace entry {


            void display(Session& session);
            void search(Session& session);

            void add(Session& session);


        }


        namespace category {


            void display(Session& session);
            void search(Session& session);

            void add(Session& session);


        }


        void initialize(Session& session);


    }

}


#endif //PSWMANAGER_SPOOKY_ROAD_COMMUNICATION_H
