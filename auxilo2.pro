TEMPLATE        = subdirs
CONFIG          += ordered

OBJECTS_DIR     = build/obj/
MOC_DIR         = build/moc/
DESTDIR         = build/bin/

SUBDIRS         = source/utils/messages \
                  source/utils/messagegroup \
                  source/examples/messagegroupexample
