#include <gio/gio.h>

#if defined (__ELF__) && ( __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 6))
# define SECTION __attribute__ ((section (".gresource.res"), aligned (8)))
#else
# define SECTION
#endif

#ifdef _MSC_VER
static const SECTION union { const guint8 data[645]; const double alignment; void * const ptr;}  res_resource_data = { {
  0107, 0126, 0141, 0162, 0151, 0141, 0156, 0164, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 
  0030, 0000, 0000, 0000, 0310, 0000, 0000, 0000, 0000, 0000, 0000, 0050, 0006, 0000, 0000, 0000, 
  0000, 0000, 0000, 0000, 0001, 0000, 0000, 0000, 0002, 0000, 0000, 0000, 0004, 0000, 0000, 0000, 
  0004, 0000, 0000, 0000, 0005, 0000, 0000, 0000, 0342, 0227, 0262, 0300, 0001, 0000, 0000, 0000, 
  0310, 0000, 0000, 0000, 0003, 0000, 0114, 0000, 0314, 0000, 0000, 0000, 0320, 0000, 0000, 0000, 
  0225, 0144, 0051, 0316, 0005, 0000, 0000, 0000, 0320, 0000, 0000, 0000, 0005, 0000, 0114, 0000, 
  0330, 0000, 0000, 0000, 0334, 0000, 0000, 0000, 0324, 0265, 0002, 0000, 0377, 0377, 0377, 0377, 
  0334, 0000, 0000, 0000, 0001, 0000, 0114, 0000, 0340, 0000, 0000, 0000, 0344, 0000, 0000, 0000, 
  0172, 0236, 0322, 0224, 0000, 0000, 0000, 0000, 0344, 0000, 0000, 0000, 0013, 0000, 0166, 0000, 
  0360, 0000, 0000, 0000, 0155, 0002, 0000, 0000, 0214, 0156, 0213, 0013, 0002, 0000, 0000, 0000, 
  0155, 0002, 0000, 0000, 0004, 0000, 0114, 0000, 0164, 0002, 0000, 0000, 0170, 0002, 0000, 0000, 
  0041, 0065, 0233, 0337, 0004, 0000, 0000, 0000, 0170, 0002, 0000, 0000, 0005, 0000, 0114, 0000, 
  0200, 0002, 0000, 0000, 0204, 0002, 0000, 0000, 0165, 0151, 0057, 0000, 0003, 0000, 0000, 0000, 
  0144, 0145, 0155, 0157, 0057, 0000, 0000, 0000, 0000, 0000, 0000, 0000, 0057, 0000, 0000, 0000, 
  0004, 0000, 0000, 0000, 0142, 0165, 0164, 0164, 0157, 0156, 0137, 0061, 0056, 0165, 0151, 0000, 
  0116, 0005, 0000, 0000, 0001, 0000, 0000, 0000, 0170, 0332, 0305, 0224, 0115, 0117, 0303, 0060, 
  0014, 0206, 0357, 0373, 0025, 0301, 0110, 0334, 0102, 0031, 0022, 0010, 0211, 0166, 0223, 0070, 
  0214, 0313, 0216, 0343, 0214, 0262, 0324, 0335, 0002, 0151, 0122, 0045, 0356, 0324, 0375, 0173, 
  0122, 0112, 0245, 0256, 0133, 0067, 0066, 0016, 0334, 0222, 0330, 0257, 0077, 0036, 0107, 0216, 
  0247, 0125, 0256, 0331, 0006, 0235, 0127, 0326, 0044, 0060, 0276, 0275, 0003, 0206, 0106, 0332, 
  0124, 0231, 0125, 0002, 0157, 0213, 0031, 0177, 0202, 0351, 0144, 0024, 0053, 0103, 0350, 0062, 
  0041, 0161, 0062, 0142, 0054, 0046, 0314, 0013, 0055, 0010, 0231, 0324, 0302, 0373, 0004, 0136, 
  0112, 0042, 0153, 0306, 0300, 0012, 0341, 0320, 0120, 0002, 0257, 0364, 0331, 0274, 0101, 0355, 
  0037, 0024, 0205, 0263, 0005, 0072, 0332, 0062, 0043, 0162, 0114, 0140, 0243, 0274, 0132, 0152, 
  0204, 0311, 0302, 0225, 0030, 0107, 0255, 0365, 0260, 0263, 0024, 0206, 0147, 0126, 0226, 0376, 
  0167, 0356, 0016, 0045, 0252, 0015, 0172, 0236, 0142, 0046, 0112, 0115, 0003, 0052, 0271, 0126, 
  0072, 0155, 0316, 0341, 0146, 0227, 0037, 0050, 0251, 0355, 0247, 0056, 0337, 0126, 0320, 0132, 
  0317, 0254, 0377, 0104, 0017, 0063, 0241, 0375, 0141, 0315, 0025, 0347, 0373, 0231, 0204, 0126, 
  0253, 0200, 0321, 0223, 0160, 0324, 0121, 0061, 0316, 0117, 0050, 0255, 0123, 0141, 0026, 0202, 
  0124, 0075, 0205, 0060, 0141, 0122, 0122, 0350, 0301, 0010, 0073, 0070, 0016, 0043, 0231, 0213, 
  0045, 0152, 0140, 0052, 0115, 0100, 0111, 0153, 0336, 0011, 0053, 0202, 0256, 0344, 0002, 0116, 
  0227, 0262, 0032, 0354, 0132, 0067, 0105, 0222, 0023, 0306, 0207, 0057, 0052, 0102, 0366, 0004, 
  0266, 0030, 0142, 0335, 0134, 0127, 0370, 0170, 0377, 0360, 0074, 0204, 0340, 0073, 0244, 0247, 
  0255, 0306, 0335, 0267, 0006, 0117, 0115, 0341, 0047, 0105, 0335, 0174, 0146, 0015, 0101, 0324, 
  0023, 0107, 0173, 0352, 0070, 0152, 0050, 0166, 0100, 0107, 0075, 0322, 0147, 0222, 0377, 0167, 
  0350, 0075, 0335, 0172, 0340, 0177, 0376, 0145, 0122, 0265, 0144, 0121, 0337, 0331, 0134, 0171, 
  0072, 0072, 0257, 0136, 0114, 0037, 0326, 0226, 0106, 0256, 0225, 0101, 0236, 0333, 0364, 0050, 
  0203, 0223, 0263, 0331, 0165, 0350, 0030, 0343, 0250, 0135, 0200, 0141, 0063, 0106, 0235, 0325, 
  0370, 0005, 0076, 0017, 0230, 0073, 0000, 0050, 0165, 0165, 0141, 0171, 0051, 0146, 0165, 0156, 
  0057, 0000, 0000, 0000, 0005, 0000, 0000, 0000, 0142, 0151, 0154, 0157, 0057, 0000, 0000, 0000, 
  0001, 0000, 0000, 0000
} };
#else /* _MSC_VER */
static const SECTION union { const guint8 data[645]; const double alignment; void * const ptr;}  res_resource_data = {
  "\107\126\141\162\151\141\156\164\000\000\000\000\000\000\000\000"
  "\030\000\000\000\310\000\000\000\000\000\000\050\006\000\000\000"
  "\000\000\000\000\001\000\000\000\002\000\000\000\004\000\000\000"
  "\004\000\000\000\005\000\000\000\342\227\262\300\001\000\000\000"
  "\310\000\000\000\003\000\114\000\314\000\000\000\320\000\000\000"
  "\225\144\051\316\005\000\000\000\320\000\000\000\005\000\114\000"
  "\330\000\000\000\334\000\000\000\324\265\002\000\377\377\377\377"
  "\334\000\000\000\001\000\114\000\340\000\000\000\344\000\000\000"
  "\172\236\322\224\000\000\000\000\344\000\000\000\013\000\166\000"
  "\360\000\000\000\155\002\000\000\214\156\213\013\002\000\000\000"
  "\155\002\000\000\004\000\114\000\164\002\000\000\170\002\000\000"
  "\041\065\233\337\004\000\000\000\170\002\000\000\005\000\114\000"
  "\200\002\000\000\204\002\000\000\165\151\057\000\003\000\000\000"
  "\144\145\155\157\057\000\000\000\000\000\000\000\057\000\000\000"
  "\004\000\000\000\142\165\164\164\157\156\137\061\056\165\151\000"
  "\116\005\000\000\001\000\000\000\170\332\305\224\115\117\303\060"
  "\014\206\357\373\025\301\110\334\102\031\022\010\211\166\223\070"
  "\214\313\216\343\214\262\324\335\002\151\122\045\356\324\375\173"
  "\122\112\245\256\133\067\066\016\334\222\330\257\077\036\107\216"
  "\247\125\256\331\006\235\127\326\044\060\276\275\003\206\106\332"
  "\124\231\125\002\157\213\031\177\202\351\144\024\053\103\350\062"
  "\041\161\062\142\054\046\314\013\055\010\231\324\302\373\004\136"
  "\112\042\153\306\300\012\341\320\120\002\257\364\331\274\101\355"
  "\037\024\205\263\005\072\332\062\043\162\114\140\243\274\132\152"
  "\204\311\302\225\030\107\255\365\260\263\024\206\147\126\226\376"
  "\167\356\016\045\252\015\172\236\142\046\112\115\003\052\271\126"
  "\072\155\316\341\146\227\037\050\251\355\247\056\337\126\320\132"
  "\317\254\377\104\017\063\241\375\141\315\025\347\373\231\204\126"
  "\253\200\321\223\160\324\121\061\316\117\050\255\123\141\026\202"
  "\124\075\205\060\141\122\122\350\301\010\073\070\016\043\231\213"
  "\045\152\140\052\115\100\111\153\336\011\053\202\256\344\002\116"
  "\227\262\032\354\132\067\105\222\023\306\207\057\052\102\366\004"
  "\266\030\142\335\134\127\370\170\377\360\074\204\340\073\244\247"
  "\255\306\335\267\006\117\115\341\047\105\335\174\146\015\101\324"
  "\023\107\173\352\070\152\050\166\100\107\075\322\147\222\377\167"
  "\350\075\335\172\340\177\376\145\122\265\144\121\337\331\134\171"
  "\072\072\257\136\114\037\326\226\106\256\225\101\236\333\364\050"
  "\203\223\263\331\165\350\030\343\250\135\200\141\063\106\235\325"
  "\370\005\076\017\230\073\000\050\165\165\141\171\051\146\165\156"
  "\057\000\000\000\005\000\000\000\142\151\154\157\057\000\000\000"
  "\001\000\000\000" };
#endif /* !_MSC_VER */

static GStaticResource static_resource = { res_resource_data.data, sizeof (res_resource_data.data) - 1 /* nul terminator */, NULL, NULL, NULL };

G_MODULE_EXPORT
GResource *res_get_resource (void);
GResource *res_get_resource (void)
{
  return g_static_resource_get_resource (&static_resource);
}
/*
  If G_HAS_CONSTRUCTORS is true then the compiler support *both* constructors and
  destructors, in a usable way, including e.g. on library unload. If not you're on
  your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER) && (_MSC_VER >= 1500)
/* Visual studio 2008 and later has _Pragma */

#include <stdlib.h>

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for x86 and x64/ARM/ARM64 programs, as symbols on x86 are prefixed
 * with an underscore but symbols on x64/ARM/ARM64 are not.
 */
#ifdef _M_IX86
#define G_MSVC_SYMBOL_PREFIX "_"
#else
#define G_MSVC_SYMBOL_PREFIX ""
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined (_MSC_VER)

#define G_HAS_CONSTRUCTORS 1

/* Pre Visual studio 2008 must use #pragma section */
#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _wrapper(void) { _func(); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (*p)(void) = _func ## _wrapper;

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _constructor(void) { atexit (_func); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#endif

#endif /* __GTK_DOC_IGNORE__ */

#ifdef G_HAS_CONSTRUCTORS

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(resresource_constructor)
#endif
G_DEFINE_CONSTRUCTOR(resresource_constructor)
#ifdef G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(resresource_destructor)
#endif
G_DEFINE_DESTRUCTOR(resresource_destructor)

#else
#warning "Constructor not supported on this compiler, linking in resources will not work"
#endif

static void resresource_constructor (void)
{
  g_static_resource_init (&static_resource);
}

static void resresource_destructor (void)
{
  g_static_resource_fini (&static_resource);
}
