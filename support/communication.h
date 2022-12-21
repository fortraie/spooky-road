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

        void initialize(Session& session);


//        namespace entriesManagement {
//
//
//            void display_entries(Session& session);
//            void search_entries(Session& session);
//
//            void add_entry(Session& session);
//            void remove_entry(Entry entry);
//            void modify_entry(Entry entry);
//
//
//        }
//
//
//        namespace categoriesManagement {
//
//
//            void display_categories(Session& session);
//            void search_categories(Session& session);
//
//            void add_category(Session& session);
//            void remove_category(Category category);
//            void modify_category(Category category);
//
//
//        }


    }

}


#endif //PSWMANAGER_SPOOKY_ROAD_COMMUNICATION_H
