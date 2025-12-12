#include <ibus.h>
// #include "engine.h"

static IBusBus *bus = NULL;
static IBusFactory *factory = NULL;

static gboolean is_key_held_down = FALSE;

static void engine_enable_cb(IBusEngine *engine) {
    system("notify-send 'enabling'");
    // g_engine = engine;
}

static void engine_disable_cb(IBusEngine *engine) {
    system("notify-send 'disabling'");
}

static gboolean key_event_cb(IBusEngine *engine, guint keyval, guint keycode, guint state) {
    if (keyval == IBUS_KEY_semicolon) {
        if (!(state & IBUS_RELEASE_MASK)) {
            ibus_engine_update_preedit_text(engine, ibus_text_new_from_static_string("nope!"), 0, TRUE);
            ibus_engine_show_preedit_text(engine);
            is_key_held_down = TRUE;
        } else {
            ibus_engine_hide_preedit_text(engine);
            is_key_held_down = FALSE;
        }
        return TRUE;
    }
    
    if (keyval == IBUS_KEY_space) {
        IBusText *surrounding_text;
        guint cursor_pos;
        guint anchor_pos;
        ibus_engine_get_surrounding_text(engine, &surrounding_text, &cursor_pos, &anchor_pos);
        int pos = cursor_pos;
        if (cursor_pos != 0) {
            if (surrounding_text->text[pos-1] == ' ') {
                return TRUE;
            }
        }
    }

    if (is_key_held_down) {
        return TRUE;
    }

    if (keycode == 12 && state & IBUS_SHIFT_MASK && state & IBUS_MOD4_MASK) { // 12 is the minus or underscore keycode. MOD4 is the modifier of LeftSuper
        if (!(state & IBUS_RELEASE_MASK)) {
            // Super wide dash
            ibus_engine_commit_text(engine, ibus_text_new_from_static_string("————————————————————————————————————————————————————————————————————"));
            return TRUE;
        }
    }

    // system(g_strdup_printf("notify-send '%s' '%i %x'",ibus_keyval_name(keyval), keycode ,state));
    return FALSE;
}

static IBusEngine *create_engine_cb(IBusFactory *factory, gchar *engine_name, gpointer user_data) {
    system("notify-send 'creating engine.'");

    IBusEngine *engine;
    gchar *path = g_strdup_printf("/org/freedesktop/IBus/Engine/%i", 1);
    engine = ibus_engine_new(engine_name,path,ibus_bus_get_connection(bus));
    g_signal_connect(engine, "process-key-event", G_CALLBACK(key_event_cb), NULL);
    g_signal_connect(engine, "enable", G_CALLBACK(engine_enable_cb), NULL);
    g_signal_connect(engine, "disable", G_CALLBACK(engine_disable_cb), NULL);

    return engine;

}

static void ibus_disconnected_cb(IBusBus *bus, gpointer  user_data) {
    system("notify-send leaving main");
    ibus_quit ();
}

int main(int argc, char **argv) {
    ibus_init();

    bus = ibus_bus_new ();
    g_object_ref_sink(bus);
    g_signal_connect(bus, "disconnected", G_CALLBACK(ibus_disconnected_cb), NULL);
	
    factory = ibus_factory_new(ibus_bus_get_connection(bus));
    g_object_ref_sink(factory);
    g_signal_connect(factory, "create-engine", G_CALLBACK(create_engine_cb), NULL);
    ibus_factory_add_engine(factory, "ibustest", IBUS_TYPE_ENGINE);

    IBusComponent *component;

    component = ibus_component_new("org.freedesktop.IBus.ibustest", "IbusTest", "0.1.0", "GPL", "", "", "", "ibus-tmpl");
    ibus_component_add_engine(component, ibus_engine_desc_new("ibustest", "Ibus Test", "A small test ibus input method.", "en", "", "", "default", "us"));
    ibus_bus_register_component(bus, component);
    ibus_main();
}