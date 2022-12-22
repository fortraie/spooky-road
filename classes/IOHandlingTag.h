#ifndef PSWMANAGER_SPOOKY_ROAD_IOHANDLINGTAG_H
#define PSWMANAGER_SPOOKY_ROAD_IOHANDLINGTAG_H


enum class IOHandlingTag {


    X_SHIFT = 1,

    PASSWORD = X_SHIFT + 1,
    DEL_PASSWORD = PASSWORD + X_SHIFT,

    TIMESTAMP = DEL_PASSWORD + 1,
    DEL_TIMESTAMP = TIMESTAMP + X_SHIFT,

    ENTRY = DEL_TIMESTAMP + 1,
    DEL_ENTRY = ENTRY + X_SHIFT,

    CATEGORY = DEL_ENTRY + 1,
    DEL_CATEGORY = CATEGORY + X_SHIFT,

    GENERATIONCONFIG = DEL_CATEGORY + 1,
    DEL_GENERATIONCONFIG = GENERATIONCONFIG + X_SHIFT,


};


#endif //PSWMANAGER_SPOOKY_ROAD_IOHANDLINGTAG_H
