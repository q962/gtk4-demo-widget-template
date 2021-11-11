#include "button_1.h"

typedef struct _Button1Private {
	GtkLabel* icon_text;
	GtkLabel* text;
} Button1Private;

enum Property
{
	PROPERTY_ICON_TEXT = 1,
	PROPERTY_TEXT,
	N_PROPERTY
};

// G_DEFINE_TYPE( Button1, button_1, GTK_TYPE_BUTTON )
G_DEFINE_TYPE_WITH_PRIVATE( Button1, button_1, GTK_TYPE_BUTTON )

static void _get_property( GObject* object, guint property_id, GValue* value, GParamSpec* pspec );
static void _set_property( GObject* object, guint property_id, const GValue* value, GParamSpec* pspec );

static void button_1_class_init( Button1Class* klass )
{
	GObjectClass* base_class = G_OBJECT_CLASS( klass );
	base_class->set_property = _set_property;
	base_class->get_property = _get_property;

	GParamSpec* pspec[ N_PROPERTY ] = {};

	pspec[ PROPERTY_ICON_TEXT ] =
	  g_param_spec_string( "icon-text",                         // 属性名，可以使用 -
	                       "Head node",                         // 不知道干嘛的，似乎是描述
	                       "The head node of the double list",  // 似乎是详细描述
	                       "icon-text",                         //默认值
	                       G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS );

	pspec[ PROPERTY_TEXT ] =
	  g_param_spec_string( "text",
	                       "Head node",
	                       "The head node of the double list",
	                       "text",
	                       G_PARAM_READABLE | G_PARAM_WRITABLE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS );
	// 作用，将属性名与属性id绑定，在调用处使用属性名，在 _set_property/_get_propert 变成属性 id
	g_object_class_install_properties( base_class, N_PROPERTY, pspec );

	gtk_widget_class_set_template_from_resource( GTK_WIDGET_CLASS( klass ), "/fun/bilo/demo/ui/button_1.ui" );
	// 调用 gtk_widget_get_template_child，设置私有属性。ID 如有需要使用下划线，包括 UI 描述文件
	gtk_widget_class_bind_template_child_private( GTK_WIDGET_CLASS( klass ), Button1, icon_text );
	gtk_widget_class_bind_template_child_private( GTK_WIDGET_CLASS( klass ), Button1, text );
}

static void button_1_init( Button1* self )
{
	Button1Private* priv = button_1_get_instance_private( self );
	gtk_widget_init_template( GTK_WIDGET( self ) );
}

void _set_property( GObject* object, guint property_id, const GValue* value, GParamSpec* pspec )
{
	Button1*        self = BUTTON_1( object );
	Button1Private* priv = button_1_get_instance_private( self );

	switch ( property_id ) {
		case PROPERTY_ICON_TEXT: gtk_label_set_label( priv->icon_text, g_value_get_string( value ) ); break;
		case PROPERTY_TEXT: gtk_label_set_label( priv->text, g_value_get_string( value ) ); break;
		default: G_OBJECT_WARN_INVALID_PROPERTY_ID( object, property_id, pspec ); break;
	}
}

void _get_property( GObject* object, guint property_id, GValue* value, GParamSpec* pspec )
{
	Button1*        self = BUTTON_1( object );
	Button1Private* priv = button_1_get_instance_private( self );

	switch ( property_id ) {
		case PROPERTY_ICON_TEXT: g_value_set_string( value, gtk_label_get_label( priv->icon_text ) ); break;
		case PROPERTY_TEXT: g_value_set_string( value, gtk_label_get_label( priv->text ) ); break;
		default: G_OBJECT_WARN_INVALID_PROPERTY_ID( object, property_id, pspec ); break;
	}
}

GtkWidget* button_1_new()
{
	return g_object_new( TYPE_BUTTON_1, NULL );
}

const char* button_1_get_text( Button1* self )
{
	g_return_val_if_fail( self, NULL );

	Button1Private* priv = button_1_get_instance_private( self );

	return gtk_label_get_label( priv->text );
}
