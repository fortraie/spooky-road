//
// Created by Jakub Kowalewski on 20/12/2022.
//

#ifndef PSWMANAGER_SPOOKY_ROAD_COMMUNICATION_H
#define PSWMANAGER_SPOOKY_ROAD_COMMUNICATION_H


#include "../classes/Session.h"

namespace com {

    void initialize();
    void returning_user();
    void new_user();

    namespace returningUser {
        void password_notification(Session& session, bool status);
        void timestamp_notification(Session& session);

        void initialize(Session& session);

        void display_passwords(Session& session);
        void search_passwords(Session& session);
        void add_password(Session& session);

        void display_categories(Session& session);
        void search_categories(Session& session);
        void add_category(Session& session);
    }

}
#endif //PSWMANAGER_SPOOKY_ROAD_COMMUNICATION_H
