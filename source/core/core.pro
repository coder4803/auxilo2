TEMPLATE        = subdirs
CONFIG          += ordered

OBJECTS_DIR     = build/obj/
MOC_DIR         = build/moc/
DESTDIR         = build/bin/

SUBDIRS         = stateholder \
                  confmanager \
                  plugininterfaces \
                  interface \
                  messagebroker
