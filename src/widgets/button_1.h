#pragma once

#include "../stdafx.h"

typedef struct _Button1 {
	GtkButton parent_instance;
} Button1;

typedef struct _Button1Class {
	GtkButtonClass parent_class;
} Button1Class;

#define TYPE_BUTTON_1 ( button_1_get_type() )
// 类型转换
#define BUTTON_1( object ) ( G_TYPE_CHECK_INSTANCE_CAST( ( object ), TYPE_BUTTON_1, Button1 ) )

// G_DECLARE_FINAL_TYPE( Button1, button_1, , BUTTON_1, GtkButton )

GType button_1_get_type();

GtkWidget* button_1_new();

const char* button_1_get_text( Button1* );