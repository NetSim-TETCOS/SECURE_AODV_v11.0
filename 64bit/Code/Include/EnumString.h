// File name: "EnumToString.h"
#undef DECL_ENUM_ELEMENT
#undef BEGIN_ENUM
#undef END_ENUM
#undef DECL_ENUM_ELEMENT_WITH_VAL

#ifndef GENERATE_ENUM_STRINGS
	#define DECL_ENUM_ELEMENT( element ) element
	#define DECL_ENUM_ELEMENT_WITH_VAL( element, val ) element = val
	#define BEGIN_ENUM( ENUM_NAME ) typedef enum tag##ENUM_NAME
	#define END_ENUM( ENUM_NAME ) ENUM_NAME; \
			char* GetString##ENUM_NAME(enum tag##ENUM_NAME index);
#else
	typedef struct { char * desc; int type;} EnumDesc_t;
	#define DECL_ENUM_ELEMENT( element ) { #element, (int)(element) }
	#define DECL_ENUM_ELEMENT_WITH_VAL( element, val ) { #element, val }
	#define BEGIN_ENUM( ENUM_NAME ) EnumDesc_t gs_##ENUM_NAME [] =
	#define END_ENUM( ENUM_NAME ) ; \
	char* GetString##ENUM_NAME(enum tag##ENUM_NAME index) \
			{ int i; \
			for (i = 0; i < sizeof(gs_##ENUM_NAME)/sizeof(EnumDesc_t); i++) { \
				if ((int)index == gs_##ENUM_NAME [i].type) return gs_##ENUM_NAME [i].desc; } \
			return "Unknown Enum type!!"; }
#endif