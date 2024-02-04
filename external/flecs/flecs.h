// Comment out this line when using as DLL
#define flecs_STATIC
/**
 * @file flecs.h
 * @brief Flecs public API.
 *
 * This file contains the public API for Flecs.
 */

#ifndef FLECS_H
#define FLECS_H

/**
 * @defgroup c C API
 * 
 * @{
 * @}
 */

/**
 * @defgroup core Core
 * @brief Core ECS functionality (entities, storage, queries).
 * 
 * \ingroup c
 * @{
 */

/**
 * @defgroup options API defines
 * @brief Defines for customizing compile time features.
 * @{
 */

/** \def ecs_float_t 
 * Customizable precision for floating point operations */
#ifndef ecs_float_t
#define ecs_float_t float
#endif

/** \def ecs_ftime_t
 * Customizable precision for scalar time values. Change to double precision for 
 * processes that can run for a long time (e.g. longer than a day). */
#ifndef ecs_ftime_t
#define ecs_ftime_t ecs_float_t
#endif

/** \def FLECS_LEGACY
 * Define when building for C89 
 */
// #define FLECS_LEGACY

/** \def FLECS_NO_DEPRECATED_WARNINGS
 * disables deprecated warnings 
 */
#define FLECS_NO_DEPRECATED_WARNINGS

/** \def FLECS_ACCURATE_COUNTERS
 * Define to ensure that global counters used for statistics (such as the 
 * allocation counters in the OS API) are accurate in multithreaded
 * applications, at the cost of increased overhead. 
 */
// #define FLECS_ACCURATE_COUNTERS

/* Make sure provided configuration is valid */
#if defined(FLECS_DEBUG) && defined(FLECS_NDEBUG)
#error "invalid configuration: cannot both define FLECS_DEBUG and FLECS_NDEBUG"
#endif
#if defined(FLECS_DEBUG) && defined(NDEBUG)
#error "invalid configuration: cannot both define FLECS_DEBUG and NDEBUG"
#endif

/** \def FLECS_DEBUG
 * Used for input parameter checking and cheap sanity checks. There are lots of 
 * asserts in every part of the code, so this will slow down applications. 
 */
#if !defined(FLECS_DEBUG) && !defined(FLECS_NDEBUG) 
#if defined(NDEBUG)
#define FLECS_NDEBUG
#else
#define FLECS_DEBUG
#endif
#endif

/** \def FLECS_SANITIZE
 * Enables expensive checks that can detect issues early. Recommended for
 * running tests or when debugging issues. This will severely slow down code.
 */
#ifdef FLECS_SANITIZE
#ifndef FLECS_DEBUG
#define FLECS_DEBUG /* If sanitized mode is enabled, so is debug mode */
#endif
#endif

/* Tip: if you see weird behavior that you think might be a bug, make sure to
 * test with the FLECS_DEBUG or FLECS_SANITIZE flags enabled. There's a good
 * chance that this gives you more information about the issue! */

/** \def FLECS_SOFT_ASSERT 
 * Define to not abort for recoverable errors, like invalid parameters. An error
 * is still thrown to the console. This is recommended for when running inside a
 * third party runtime, such as the Unreal editor.
 * 
 * Note that internal sanity checks (ECS_INTERNAL_ERROR) will still abort a
 * process, as this gives more information than a (likely) subsequent crash.
 * 
 * When a soft assert occurs, the code will attempt to minimize the number of 
 * side effects of the failed operation, but this may not always be possible.
 * Even though an application may still be able to continue running after a soft 
 * assert, it should be treated as if in an undefined state. 
 */
// #define FLECS_SOFT_ASSERT

/** \def FLECS_KEEP_ASSERT
 * By default asserts are disabled in release mode, when either FLECS_NDEBUG or
 * NDEBUG is defined. Defining FLECS_KEEP_ASSERT ensures that asserts are not
 * disabled. This define can be combined with FLECS_SOFT_ASSERT. 
 */
// #define FLECS_KEEP_ASSERT

/** \def FLECS_CUSTOM_BUILD
 * This macro lets you customize which addons to build flecs with.
 * Without any addons Flecs is just a minimal ECS storage, but addons add 
 * features such as systems, scheduling and reflection. If an addon is disabled,
 * it is excluded from the build, so that it consumes no resources. By default
 * all addons are enabled.
 * 
 * You can customize a build by either whitelisting or blacklisting addons. To
 * whitelist addons, first define the FLECS_CUSTOM_BUILD macro, which disables
 * all addons. You can then manually select the addons you need by defining
 * their macro, like "FLECS_SYSTEM".
 * 
 * To blacklist an addon, make sure to *not* define FLECS_CUSTOM_BUILD, and
 * instead define the addons you don't need by defining FLECS_NO_<addon>, for
 * example "FLECS_NO_SYSTEM". If there are any addons that depend on the 
 * blacklisted addon, an error will be thrown during the build.
 * 
 * Note that addons can have dependencies on each other. Addons will 
 * automatically enable their dependencies. To see the list of addons that was
 * compiled in a build, enable tracing before creating the world by doing:
 *   ecs_log_set_level(0);
 * which outputs the full list of addons Flecs was compiled with.
 */
// #define FLECS_CUSTOM_BUILD

#ifndef FLECS_CUSTOM_BUILD
// #define FLECS_C          /**< C API convenience macros, always enabled */
#define FLECS_CPP           /**< C++ API */
#define FLECS_MODULE        /**< Module support */
#define FLECS_PARSER        /**< String parser for queries */
#define FLECS_PLECS         /**< ECS data definition format */
#define FLECS_RULES         /**< Constraint solver for advanced queries */
#define FLECS_SNAPSHOT      /**< Snapshot & restore ECS data */
#define FLECS_STATS         /**< Access runtime statistics */
#define FLECS_MONITOR       /**< Track runtime statistics periodically */
#define FLECS_METRICS       /**< Expose component data as statistics */
#define FLECS_ALERTS        /**< Monitor conditions for errors */
#define FLECS_SYSTEM        /**< System support */
#define FLECS_PIPELINE      /**< Pipeline support */
#define FLECS_TIMER         /**< Timer support */
#define FLECS_META          /**< Reflection support */
#define FLECS_META_C        /**< Utilities for populating reflection data */
#define FLECS_UNITS         /**< Builtin standard units */
#define FLECS_EXPR          /**< Parsing strings to/from component values */
#define FLECS_JSON          /**< Parsing JSON to/from component values */
#define FLECS_DOC           /**< Document entities & components */
#define FLECS_COREDOC       /**< Documentation for core entities & components */
#define FLECS_LOG           /**< When enabled ECS provides more detailed logs */
#define FLECS_APP           /**< Application addon */
#define FLECS_OS_API_IMPL   /**< Default implementation for OS API */
#define FLECS_HTTP          /**< Tiny HTTP server for connecting to remote UI */
#define FLECS_REST          /**< REST API for querying application data */
// #define FLECS_JOURNAL    /**< Journaling addon (disabled by default) */
#endif // ifndef FLECS_CUSTOM_BUILD

/** \def FLECS_LOW_FOOTPRINT
 * Set a number of constants to values that decrease memory footprint, at the
 * cost of decreased performance. */
// #define FLECS_LOW_FOOTPRINT
#ifdef FLECS_LOW_FOOTPRINT
#define FLECS_HI_COMPONENT_ID (16)
#define FLECS_HI_ID_RECORD_ID (16)
#define FLECS_SPARSE_PAGE_BITS (6)
#define FLECS_ENTITY_PAGE_BITS (6)
#define FLECS_USE_OS_ALLOC
#endif

/** \def FLECS_HI_COMPONENT_ID
 * This constant can be used to balance between performance and memory 
 * utilization. The constant is used in two ways:
 * - Entity ids 0..FLECS_HI_COMPONENT_ID are reserved for component ids.
 * - Used as lookup array size in table edges.
 * 
 * Increasing this value increases the size of the lookup array, which allows
 * fast table traversal, which improves performance of ECS add/remove 
 * operations. Component ids that fall outside of this range use a regular map
 * lookup, which is slower but more memory efficient. */
#ifndef FLECS_HI_COMPONENT_ID
#define FLECS_HI_COMPONENT_ID (256)
#endif

/** \def FLECS_HI_ID_RECORD_ID
 * This constant can be used to balance between performance and memory 
 * utilization. The constant is used to determine the size of the id record
 * lookup array. Id values that fall outside of this range use a regular map
 * lookup, which is slower but more memory efficient.
 */
#ifndef FLECS_HI_ID_RECORD_ID
#define FLECS_HI_ID_RECORD_ID (1024)
#endif

/** \def FLECS_SPARSE_PAGE_BITS
 * This constant is used to determine the number of bits of an id that is used
 * to determine the page index when used with a sparse set. The number of bits
 * determines the page size, which is (1 << bits).
 * Lower values decrease memory utilization, at the cost of more allocations. */
#ifndef FLECS_SPARSE_PAGE_BITS
#define FLECS_SPARSE_PAGE_BITS (12)
#endif

/** \def FLECS_ENTITY_PAGE_BITS
 * Same as FLECS_SPARSE_PAGE_BITS, but for the entity index. */
#ifndef FLECS_ENTITY_PAGE_BITS
#define FLECS_ENTITY_PAGE_BITS (12)
#endif

/** \def FLECS_USE_OS_ALLOC 
 * When enabled, Flecs will use the OS allocator provided in the OS API directly
 * instead of the builtin block allocator. This can decrease memory utilization
 * as memory will be freed more often, at the cost of decreased performance. */
// #define FLECS_USE_OS_ALLOC

/** \def FLECS_ID_DESC_MAX
 * Maximum number of ids to add ecs_entity_desc_t / ecs_bulk_desc_t */
#ifndef FLECS_ID_DESC_MAX
#define FLECS_ID_DESC_MAX (32)
#endif

/** \def FLECS_TERM_DESC_MAX 
 * Maximum number of terms in ecs_filter_desc_t */
#define FLECS_TERM_DESC_MAX (16)

/** \def FLECS_EVENT_DESC_MAX
 * Maximum number of events in ecs_observer_desc_t */
#define FLECS_EVENT_DESC_MAX (8)

/** \def FLECS_VARIABLE_COUNT_MAX
 * Maximum number of query variables per query */
#define FLECS_VARIABLE_COUNT_MAX (64)

/** \def FLECS_QUERY_SCOPE_NESTING_MAX 
 * Maximum nesting depth of query scopes */
#define FLECS_QUERY_SCOPE_NESTING_MAX (8)

/** @} */

/**
 * @file api_defines.h
 * @brief Supporting defines for the public API.
 *
 * This file contains constants / macros that are typically not used by an
 * application but support the public API, and therefore must be exposed. This
 * header should not be included by itself.
 */

#ifndef FLECS_API_DEFINES_H
#define FLECS_API_DEFINES_H

/**
 * @file api_flags.h
 * @brief Bitset flags used by internals.
 */

#ifndef FLECS_API_FLAGS_H
#define FLECS_API_FLAGS_H



////////////////////////////////////////////////////////////////////////////////
//// World flags
////////////////////////////////////////////////////////////////////////////////

#define EcsWorldQuitWorkers           (1u << 0)
#define EcsWorldReadonly              (1u << 1)
#define EcsWorldInit                  (1u << 2)
#define EcsWorldQuit                  (1u << 3)
#define EcsWorldFini                  (1u << 4)
#define EcsWorldMeasureFrameTime      (1u << 5)
#define EcsWorldMeasureSystemTime     (1u << 6)
#define EcsWorldMultiThreaded         (1u << 7)


////////////////////////////////////////////////////////////////////////////////
//// OS API flags
////////////////////////////////////////////////////////////////////////////////

#define EcsOsApiHighResolutionTimer   (1u << 0)
#define EcsOsApiLogWithColors         (1u << 1)
#define EcsOsApiLogWithTimeStamp      (1u << 2)
#define EcsOsApiLogWithTimeDelta      (1u << 3)


////////////////////////////////////////////////////////////////////////////////
//// Entity flags (set in upper bits of ecs_record_t::row)
////////////////////////////////////////////////////////////////////////////////

#define EcsEntityIsId                 (1u << 31)
#define EcsEntityIsTarget             (1u << 30)
#define EcsEntityIsTraversable        (1u << 29)


////////////////////////////////////////////////////////////////////////////////
//// Id flags (used by ecs_id_record_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsIdOnDeleteRemove            (1u << 0)
#define EcsIdOnDeleteDelete            (1u << 1)
#define EcsIdOnDeletePanic             (1u << 2)
#define EcsIdOnDeleteMask\
    (EcsIdOnDeletePanic|EcsIdOnDeleteRemove|EcsIdOnDeleteDelete)

#define EcsIdOnDeleteObjectRemove      (1u << 3)
#define EcsIdOnDeleteObjectDelete      (1u << 4)
#define EcsIdOnDeleteObjectPanic       (1u << 5)
#define EcsIdOnDeleteObjectMask\
    (EcsIdOnDeleteObjectPanic|EcsIdOnDeleteObjectRemove|\
        EcsIdOnDeleteObjectDelete)

#define EcsIdExclusive                 (1u << 6)
#define EcsIdDontInherit               (1u << 7)
#define EcsIdTraversable               (1u << 8)
#define EcsIdTag                       (1u << 9)
#define EcsIdWith                      (1u << 10)
#define EcsIdUnion                     (1u << 11)
#define EcsIdAlwaysOverride            (1u << 12)

#define EcsIdHasOnAdd                  (1u << 16) /* Same values as table flags */
#define EcsIdHasOnRemove               (1u << 17) 
#define EcsIdHasOnSet                  (1u << 18)
#define EcsIdHasUnSet                  (1u << 19)
#define EcsIdHasOnTableFill            (1u << 20)
#define EcsIdHasOnTableEmpty           (1u << 21)
#define EcsIdHasOnTableCreate          (1u << 22)
#define EcsIdHasOnTableDelete          (1u << 23)
#define EcsIdEventMask\
    (EcsIdHasOnAdd|EcsIdHasOnRemove|EcsIdHasOnSet|EcsIdHasUnSet|\
        EcsIdHasOnTableFill|EcsIdHasOnTableEmpty|EcsIdHasOnTableCreate|\
            EcsIdHasOnTableDelete)

#define EcsIdMarkedForDelete           (1u << 30)

/* Utilities for converting from flags to delete policies and vice versa */
#define ECS_ID_ON_DELETE(flags) \
    ((ecs_entity_t[]){0, EcsRemove, EcsDelete, 0, EcsPanic}\
        [((flags) & EcsIdOnDeleteMask)])
#define ECS_ID_ON_DELETE_TARGET(flags) ECS_ID_ON_DELETE(flags >> 3)
#define ECS_ID_ON_DELETE_FLAG(id) (1u << ((id) - EcsRemove))
#define ECS_ID_ON_DELETE_TARGET_FLAG(id) (1u << (3 + ((id) - EcsRemove)))


////////////////////////////////////////////////////////////////////////////////
//// Iterator flags (used by ecs_iter_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsIterIsValid                 (1u << 0u)  /* Does iterator contain valid result */
#define EcsIterNoData                  (1u << 1u)  /* Does iterator provide (component) data */
#define EcsIterIsInstanced             (1u << 2u)  /* Is iterator instanced */
#define EcsIterHasShared               (1u << 3u)  /* Does result have shared terms */
#define EcsIterTableOnly               (1u << 4u)  /* Result only populates table */
#define EcsIterEntityOptional          (1u << 5u)  /* Treat terms with entity subject as optional */
#define EcsIterNoResults               (1u << 6u)  /* Iterator has no results */
#define EcsIterIgnoreThis              (1u << 7u)  /* Only evaluate non-this terms */
#define EcsIterMatchVar                (1u << 8u)  
#define EcsIterHasCondSet              (1u << 10u) /* Does iterator have conditionally set fields */
#define EcsIterProfile                 (1u << 11u) /* Profile iterator performance */
#define EcsIterTrivialSearch           (1u << 12u) /* Trivial iterator mode */
#define EcsIterTrivialSearchNoData     (1u << 13u) /* Trivial iterator w/no data */
#define EcsIterTrivialTest             (1u << 14u) /* Trivial test mode (constrained $this) */
#define EcsIterTrivialSearchWildcard   (1u << 15u) /* Trivial search with wildcard ids */

////////////////////////////////////////////////////////////////////////////////
//// Event flags (used by ecs_event_decs_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsEventTableOnly              (1u << 4u)   /* Table event (no data, same as iter flags) */
#define EcsEventNoOnSet                (1u << 16u)  /* Don't emit OnSet/UnSet for inherited ids */

////////////////////////////////////////////////////////////////////////////////
//// Filter flags (used by ecs_filter_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsFilterMatchThis             (1u << 1u)  /* Has terms that match This */
#define EcsFilterMatchOnlyThis         (1u << 2u)  /* Has only terms that match This */
#define EcsFilterMatchPrefab           (1u << 3u)  /* Does filter match prefabs */
#define EcsFilterMatchDisabled         (1u << 4u)  /* Does filter match disabled entities */
#define EcsFilterMatchEmptyTables      (1u << 5u)  /* Does filter return empty tables */
#define EcsFilterMatchAnything         (1u << 6u)  /* False if filter has no/only Not terms */
#define EcsFilterNoData                (1u << 7u)  /* When true, data fields won't be populated */
#define EcsFilterIsInstanced           (1u << 8u)  /* Is filter instanced (see ecs_filter_desc_t) */
#define EcsFilterPopulate              (1u << 9u)  /* Populate data, ignore non-matching fields */
#define EcsFilterHasCondSet            (1u << 10u) /* Does filter have conditionally set fields */
#define EcsFilterUnresolvedByName      (1u << 11u) /* Use by-name matching for unresolved entity identifiers */
#define EcsFilterHasPred               (1u << 12u) /* Filter has equality predicates */
#define EcsFilterHasScopes             (1u << 13u) /* Filter has query scopes */
#define EcsFilterIsTrivial             (1u << 14u) /* Trivial filter */
#define EcsFilterMatchOnlySelf         (1u << 15u) /* Filter has no up traversal */
#define EcsFilterHasWildcards          (1u << 16u) /* Filter has no up traversal */
#define EcsFilterOwnsStorage           (1u << 17u) /* Is ecs_filter_t object owned by filter */
#define EcsFilterOwnsTermsStorage      (1u << 18u) /* Is terms array owned by filter */

////////////////////////////////////////////////////////////////////////////////
//// Table flags (used by ecs_table_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsTableHasBuiltins            (1u << 1u)  /* Does table have builtin components */
#define EcsTableIsPrefab               (1u << 2u)  /* Does the table store prefabs */
#define EcsTableHasIsA                 (1u << 3u)  /* Does the table have IsA relationship */
#define EcsTableHasChildOf             (1u << 4u)  /* Does the table type ChildOf relationship */
#define EcsTableHasName                (1u << 5u)  /* Does the table type have (Identifier, Name) */
#define EcsTableHasPairs               (1u << 6u)  /* Does the table type have pairs */
#define EcsTableHasModule              (1u << 7u)  /* Does the table have module data */
#define EcsTableIsDisabled             (1u << 8u)  /* Does the table type has EcsDisabled */
#define EcsTableHasCtors               (1u << 9u)
#define EcsTableHasDtors               (1u << 10u)
#define EcsTableHasCopy                (1u << 11u)
#define EcsTableHasMove                (1u << 12u)
#define EcsTableHasUnion               (1u << 13u)
#define EcsTableHasToggle              (1u << 14u)
#define EcsTableHasOverrides           (1u << 15u)

#define EcsTableHasOnAdd               (1u << 16u) /* Same values as id flags */
#define EcsTableHasOnRemove            (1u << 17u)
#define EcsTableHasOnSet               (1u << 18u)
#define EcsTableHasUnSet               (1u << 19u)
#define EcsTableHasOnTableFill         (1u << 20u)
#define EcsTableHasOnTableEmpty        (1u << 21u)
#define EcsTableHasOnTableCreate       (1u << 22u)
#define EcsTableHasOnTableDelete       (1u << 23u)

#define EcsTableHasTraversable         (1u << 25u)
#define EcsTableHasTarget              (1u << 26u)

#define EcsTableMarkedForDelete        (1u << 30u)

/* Composite table flags */
#define EcsTableHasLifecycle        (EcsTableHasCtors | EcsTableHasDtors)
#define EcsTableIsComplex           (EcsTableHasLifecycle | EcsTableHasUnion | EcsTableHasToggle)
#define EcsTableHasAddActions       (EcsTableHasIsA | EcsTableHasUnion | EcsTableHasCtors | EcsTableHasOnAdd | EcsTableHasOnSet)
#define EcsTableHasRemoveActions    (EcsTableHasIsA | EcsTableHasDtors | EcsTableHasOnRemove | EcsTableHasUnSet)


////////////////////////////////////////////////////////////////////////////////
//// Query flags (used by ecs_query_t::flags)
////////////////////////////////////////////////////////////////////////////////

#define EcsQueryHasRefs                (1u << 1u)  /* Does query have references */
#define EcsQueryIsSubquery             (1u << 2u)  /* Is query a subquery */
#define EcsQueryIsOrphaned             (1u << 3u)  /* Is subquery orphaned */
#define EcsQueryHasOutTerms            (1u << 4u)  /* Does query have out terms */
#define EcsQueryHasNonThisOutTerms     (1u << 5u)  /* Does query have non-this out terms */
#define EcsQueryHasMonitor             (1u << 6u)  /* Does query track changes */
#define EcsQueryTrivialIter            (1u << 7u)  /* Does the query require special features to iterate */


////////////////////////////////////////////////////////////////////////////////
//// Aperiodic action flags (used by ecs_run_aperiodic)
////////////////////////////////////////////////////////////////////////////////

#define EcsAperiodicEmptyTables        (1u << 1u)  /* Process pending empty table events */
#define EcsAperiodicComponentMonitors  (1u << 2u)  /* Process component monitors */
#define EcsAperiodicEmptyQueries       (1u << 4u)  /* Process empty queries */


#endif


#if defined(_WIN32) || defined(_MSC_VER)
#define ECS_TARGET_WINDOWS
#elif defined(__ANDROID__)
#define ECS_TARGET_ANDROID
#define ECS_TARGET_POSIX
#elif defined(__linux__)
#define ECS_TARGET_LINUX
#define ECS_TARGET_POSIX
#elif defined(__FreeBSD__)
#define ECS_TARGET_FREEBSD
#define ECS_TARGET_POSIX
#elif defined(__APPLE__) && defined(__MACH__)
#define ECS_TARGET_DARWIN
#define ECS_TARGET_POSIX
#elif defined(__EMSCRIPTEN__)
#define ECS_TARGET_EM
#define ECS_TARGET_POSIX
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
#define ECS_TARGET_MINGW
#endif

#if defined(_MSC_VER)
#ifndef __clang__
#define ECS_TARGET_MSVC
#endif
#endif

#if defined(__clang__)
#define ECS_TARGET_CLANG
#endif

#if defined(__GNUC__)
#define ECS_TARGET_GNU
#endif

/* Map between clang and apple clang versions, as version 13 has a difference in
 * the format of __PRETTY_FUNCTION__ which enum reflection depends on. */
#if defined(__clang__)
    #if defined(__APPLE__)
        #if __clang_major__ == 13
            #if __clang_minor__ < 1
                #define ECS_CLANG_VERSION 12
            #else
                #define ECS_CLANG_VERSION 13
            #endif
        #else
            #define ECS_CLANG_VERSION __clang_major__
        #endif
    #else
        #define ECS_CLANG_VERSION __clang_major__
    #endif
#endif

/* Ignored warnings */
#if defined(ECS_TARGET_CLANG)
/* Ignore unknown options so we don't have to care about the compiler version */
#pragma clang diagnostic ignored "-Wunknown-warning-option"
/* Warns for double or redundant semicolons. There are legitimate cases where a
 * semicolon after an empty statement is useful, for example after a macro that
 * is replaced with a code block. With this warning enabled, semicolons would 
 * only have to be added after macro's that are not code blocks, which in some
 * cases isn't possible as the implementation of a macro can be different in
 * debug/release mode. */
#pragma clang diagnostic ignored "-Wextra-semi-stmt"
/* This is valid in C99, and Flecs must be compiled as C99. */
#pragma clang diagnostic ignored "-Wdeclaration-after-statement"
/* Clang attribute to detect fallthrough isn't supported on older versions. 
 * Implicit fallthrough is still detected by gcc and ignored with "fall through"
 * comments */
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
/* This warning prevents adding a default case when all enum constants are part
 * of the switch. In C however an enum type can assume any value in the range of
 * the type, and this warning makes it harder to catch invalid enum values. */
#pragma clang diagnostic ignored "-Wcovered-switch-default"
/* This warning prevents some casts of function results to a different kind of
 * type, e.g. casting an int result to double. Not very useful in practice, as
 * it just forces the code to assign to a variable first, then cast. */
#pragma clang diagnostic ignored "-Wbad-function-cast"
/* Format strings can be passed down from other functions. */
#pragma clang diagnostic ignored "-Wformat-nonliteral"
/* Useful, but not reliable enough. It can incorrectly flag macro's as unused
 * in standalone builds. */
#pragma clang diagnostic ignored "-Wunused-macros"
#if __clang_major__ == 13
/* clang 13 can throw this warning for a define in ctype.h */
#pragma clang diagnostic ignored "-Wreserved-identifier"
#endif
/* Filenames aren't consistent across targets as they can use different casing 
 * (e.g. WinSock2 vs winsock2). */
#pragma clang diagnostic ignored "-Wnonportable-system-include-path"
/* Enum reflection relies on testing constant values that may not be valid for
 * the enumeration. */
#pragma clang diagnostic ignored "-Wenum-constexpr-conversion"
/* Very difficult to workaround this warning in C, especially for an ECS. */
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
/* This warning gets thrown when trying to cast pointer returned from dlproc */
#pragma clang diagnostic ignored "-Wcast-function-type-strict"
/* This warning can get thrown for expressions that evaluate to constants
 * in debug/release mode. */
#pragma clang diagnostic ignored "-Wconstant-logical-operand"
#elif defined(ECS_TARGET_GNU)
#ifndef __cplusplus
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#pragma GCC diagnostic ignored "-Wunused-macros"
/* This warning gets thrown *sometimes* when not all members for a struct are
 * provided in an initializer. Flecs heavily relies on descriptor structs that
 * only require partly initialization, so this warning isn't useful.
 * It doesn't introduce any safety issues (fields are guaranteed to be 0 
 * initialized), and later versions of gcc (>=11) seem to no longer throw this 
 * warning. */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

/* Standard library dependencies */
#include <assert.h>
#include <stdarg.h>
#include <string.h>

/* Non-standard but required. If not provided by platform, add manually. */
#include <stdint.h>

/* Contains macros for importing / exporting symbols */
/*
                                   )
                                  (.)
                                  .|.
                                  | |
                              _.--| |--._
                           .-';  ;`-'& ; `&.
                          \   &  ;    &   &_/
                           |"""---...---"""|
                           \ | | | | | | | /
                            `---.|.|.|.---'

 * This file is generated by bake.lang.c for your convenience. Headers of
 * dependencies will automatically show up in this file. Include bake_config.h
 * in your main project file. Do not edit! */

#ifndef FLECS_BAKE_CONFIG_H
#define FLECS_BAKE_CONFIG_H

/* Headers of public dependencies */
/* No dependencies */

/* Convenience macro for exporting symbols */
#ifndef flecs_STATIC
#if defined(flecs_EXPORTS) && (defined(_MSC_VER) || defined(__MINGW32__))
  #define FLECS_API __declspec(dllexport)
#elif defined(flecs_EXPORTS)
  #define FLECS_API __attribute__((__visibility__("default")))
#elif defined(_MSC_VER)
  #define FLECS_API __declspec(dllimport)
#else
  #define FLECS_API
#endif
#else
  #define FLECS_API
#endif

#endif




#ifdef __BAKE_LEGACY__
#define FLECS_LEGACY
#endif

/* Some symbols are only exported when building in debug build, to enable
 * whitebox testing of internal datastructures */
#ifndef FLECS_NDEBUG
#define FLECS_DBG_API FLECS_API
#else
#define FLECS_DBG_API
#endif


////////////////////////////////////////////////////////////////////////////////
//// Language support defines
////////////////////////////////////////////////////////////////////////////////

#ifndef FLECS_LEGACY
#include <stdbool.h>
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

/* The API uses the native bool type in C++, or a custom one in C */
#if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)
#undef bool
#undef true
#undef false
typedef char bool;
#define false 0
#define true !false
#endif

/* Utility types to indicate usage as bitmask */
typedef uint8_t ecs_flags8_t;
typedef uint16_t ecs_flags16_t;
typedef uint32_t ecs_flags32_t;
typedef uint64_t ecs_flags64_t;

/* Keep unsigned integers out of the codebase as they do more harm than good */
typedef int32_t ecs_size_t;

/* Allocator type */
typedef struct ecs_allocator_t ecs_allocator_t;

#define ECS_SIZEOF(T) ECS_CAST(ecs_size_t, sizeof(T))

/* Use alignof in C++, or a trick in C. */
#if defined(ECS_TARGET_MSVC)
#define ECS_ALIGNOF(T) (int64_t)__alignof(T)
#elif defined(ECS_TARGET_GNU)
#define ECS_ALIGNOF(T) (int64_t)__alignof__(T)
#else
#define ECS_ALIGNOF(T) ((int64_t)&((struct { char c; T d; } *)0)->d)
#endif

#ifndef FLECS_NO_DEPRECATED_WARNINGS
#if defined(ECS_TARGET_GNU)
#define ECS_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(ECS_TARGET_MSVC)
#define ECS_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define ECS_DEPRECATED(msg)
#endif
#else
#define ECS_DEPRECATED(msg)
#endif

#define ECS_ALIGN(size, alignment) (ecs_size_t)((((((size_t)size) - 1) / ((size_t)alignment)) + 1) * ((size_t)alignment))

/* Simple utility for determining the max of two values */
#define ECS_MAX(a, b) (((a) > (b)) ? a : b)
#define ECS_MIN(a, b) (((a) < (b)) ? a : b)

/* Abstraction on top of C-style casts so that C functions can be used in C++
 * code without producing warnings */
#ifndef __cplusplus
#define ECS_CAST(T, V) ((T)(V))
#else
#define ECS_CAST(T, V) (static_cast<T>(V))
#endif

/* Utility macro for doing const casts without warnings */
#ifndef __cplusplus
#define ECS_CONST_CAST(type, value) ((type)(uintptr_t)(value))
#else
#define ECS_CONST_CAST(type, value) (const_cast<type>(value))
#endif

/* Utility macro for doing pointer casts without warnings */
#ifndef __cplusplus
#define ECS_PTR_CAST(type, value) ((type)(uintptr_t)(value))
#else
#define ECS_PTR_CAST(type, value) (reinterpret_cast<type>(value))
#endif

/* Utility macro's to do bitwise comparisons between floats without warnings */
#define ECS_EQ(a, b) (ecs_os_memcmp(&(a), &(b), sizeof(a)) == 0)
#define ECS_NEQ(a, b) (!ECS_EQ(a, b))
#define ECS_EQZERO(a) ECS_EQ(a, (uint64_t){0})
#define ECS_NEQZERO(a) ECS_NEQ(a, (uint64_t){0})

#define ECS_CONCAT(a, b) a ## b

////////////////////////////////////////////////////////////////////////////////
//// Magic numbers for sanity checking
////////////////////////////////////////////////////////////////////////////////

/* Magic number to identify the type of the object */
#define ecs_world_t_magic     (0x65637377)
#define ecs_stage_t_magic     (0x65637373)
#define ecs_query_t_magic     (0x65637371)
#define ecs_rule_t_magic      (0x65637375)
#define ecs_table_t_magic     (0x65637374)
#define ecs_filter_t_magic    (0x65637366)
#define ecs_trigger_t_magic   (0x65637372)
#define ecs_observer_t_magic  (0x65637362)


////////////////////////////////////////////////////////////////////////////////
//// Entity id macros
////////////////////////////////////////////////////////////////////////////////

#define ECS_ROW_MASK                  (0x0FFFFFFFu)
#define ECS_ROW_FLAGS_MASK            (~ECS_ROW_MASK)
#define ECS_RECORD_TO_ROW(v)          (ECS_CAST(int32_t, (ECS_CAST(uint32_t, v) & ECS_ROW_MASK)))
#define ECS_RECORD_TO_ROW_FLAGS(v)    (ECS_CAST(uint32_t, v) & ECS_ROW_FLAGS_MASK)
#define ECS_ROW_TO_RECORD(row, flags) (ECS_CAST(uint32_t, (ECS_CAST(uint32_t, row) | (flags))))

#define ECS_ID_FLAGS_MASK             (0xFFull << 60)
#define ECS_ENTITY_MASK               (0xFFFFFFFFull)
#define ECS_GENERATION_MASK           (0xFFFFull << 32)
#define ECS_GENERATION(e)             ((e & ECS_GENERATION_MASK) >> 32)
#define ECS_GENERATION_INC(e)         ((e & ~ECS_GENERATION_MASK) | ((0xFFFF & (ECS_GENERATION(e) + 1)) << 32))
#define ECS_COMPONENT_MASK            (~ECS_ID_FLAGS_MASK)
#define ECS_HAS_ID_FLAG(e, flag)      ((e) & ECS_##flag)
#define ECS_IS_PAIR(id)               (((id) & ECS_ID_FLAGS_MASK) == ECS_PAIR)
#define ECS_PAIR_FIRST(e)             (ecs_entity_t_hi(e & ECS_COMPONENT_MASK))
#define ECS_PAIR_SECOND(e)            (ecs_entity_t_lo(e))
#define ECS_HAS_RELATION(e, rel)      (ECS_HAS_ID_FLAG(e, PAIR) && (ECS_PAIR_FIRST(e) == rel))


////////////////////////////////////////////////////////////////////////////////
//// Convert between C typenames and variables
////////////////////////////////////////////////////////////////////////////////

/** Translate C type to id. */
#define ecs_id(T) FLECS_ID##T##ID_


////////////////////////////////////////////////////////////////////////////////
//// Utilities for working with pair identifiers
////////////////////////////////////////////////////////////////////////////////

#define ecs_entity_t_lo(value) ECS_CAST(uint32_t, value)
#define ecs_entity_t_hi(value) ECS_CAST(uint32_t, (value) >> 32)
#define ecs_entity_t_comb(lo, hi) ((ECS_CAST(uint64_t, hi) << 32) + ECS_CAST(uint32_t, lo))

#define ecs_pair(pred, obj) (ECS_PAIR | ecs_entity_t_comb(obj, pred))
#define ecs_pair_t(pred, obj) (ECS_PAIR | ecs_entity_t_comb(obj, ecs_id(pred)))
#define ecs_pair_first(world, pair) ecs_get_alive(world, ECS_PAIR_FIRST(pair))
#define ecs_pair_second(world, pair) ecs_get_alive(world, ECS_PAIR_SECOND(pair))
#define ecs_pair_relation ecs_pair_first
#define ecs_pair_object ecs_pair_second

#define ecs_poly_id(tag) ecs_pair(ecs_id(EcsPoly), tag)


////////////////////////////////////////////////////////////////////////////////
//// Debug macros
////////////////////////////////////////////////////////////////////////////////

#ifndef FLECS_NDEBUG
#define ECS_TABLE_LOCK(world, table) ecs_table_lock(world, table)
#define ECS_TABLE_UNLOCK(world, table) ecs_table_unlock(world, table)
#else
#define ECS_TABLE_LOCK(world, table)
#define ECS_TABLE_UNLOCK(world, table)
#endif


////////////////////////////////////////////////////////////////////////////////
//// Actions that drive iteration
////////////////////////////////////////////////////////////////////////////////

#define EcsIterNextYield  (0)   /* Move to next table, yield current */
#define EcsIterYield      (-1)  /* Stay on current table, yield */
#define EcsIterNext  (1)   /* Move to next table, don't yield */

////////////////////////////////////////////////////////////////////////////////
//// Convenience macros for ctor, dtor, move and copy
////////////////////////////////////////////////////////////////////////////////

#ifndef FLECS_LEGACY

/* Constructor/Destructor convenience macro */
#define ECS_XTOR_IMPL(type, postfix, var, ...)\
    void type##_##postfix(\
        void *_ptr,\
        int32_t _count,\
        const ecs_type_info_t *type_info)\
    {\
        (void)_ptr;\
        (void)_count;\
        (void)type_info;\
        for (int32_t i = 0; i < _count; i ++) {\
            type *var = &((type*)_ptr)[i];\
            (void)var;\
            __VA_ARGS__\
        }\
    }

/* Copy convenience macro */
#define ECS_COPY_IMPL(type, dst_var, src_var, ...)\
    void type##_##copy(\
        void *_dst_ptr,\
        const void *_src_ptr,\
        int32_t _count,\
        const ecs_type_info_t *type_info)\
    {\
        (void)_dst_ptr;\
        (void)_src_ptr;\
        (void)_count;\
        (void)type_info;\
        for (int32_t i = 0; i < _count; i ++) {\
            type *dst_var = &((type*)_dst_ptr)[i];\
            const type *src_var = &((const type*)_src_ptr)[i];\
            (void)dst_var;\
            (void)src_var;\
            __VA_ARGS__\
        }\
    }

/* Move convenience macro */
#define ECS_MOVE_IMPL(type, dst_var, src_var, ...)\
    void type##_##move(\
        void *_dst_ptr,\
        void *_src_ptr,\
        int32_t _count,\
        const ecs_type_info_t *type_info)\
    {\
        (void)_dst_ptr;\
        (void)_src_ptr;\
        (void)_count;\
        (void)type_info;\
        for (int32_t i = 0; i < _count; i ++) {\
            type *dst_var = &((type*)_dst_ptr)[i];\
            type *src_var = &((type*)_src_ptr)[i];\
            (void)dst_var;\
            (void)src_var;\
            __VA_ARGS__\
        }\
    }

#define ECS_HOOK_IMPL(type, func, var, ...)\
    void func(ecs_iter_t *_it)\
    {\
        for (int32_t i = 0; i < _it->count; i ++) {\
            ecs_entity_t entity = _it->entities[i];\
            type *var = &((type*)_it->ptrs[0])[i];\
            (void)entity;\
            (void)var;\
            __VA_ARGS__\
        }\
    }

#endif


#endif

/**
 * @file vec.h
 * @brief Vector with allocator support.
 */

#ifndef FLECS_VEC_H
#define FLECS_VEC_H



/** A component column. */
typedef struct ecs_vec_t {
    void *array;
    int32_t count;
    int32_t size;
#ifdef FLECS_SANITIZE
    ecs_size_t elem_size;
#endif
} ecs_vec_t;

FLECS_API
ecs_vec_t* ecs_vec_init(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem_count);

#define ecs_vec_init_t(allocator, vec, T, elem_count) \
    ecs_vec_init(allocator, vec, ECS_SIZEOF(T), elem_count)

FLECS_API
void ecs_vec_init_if(
    ecs_vec_t *vec,
    ecs_size_t size);

#define ecs_vec_init_if_t(vec, T) \
    ecs_vec_init_if(vec, ECS_SIZEOF(T))

FLECS_API
void ecs_vec_fini(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size);

#define ecs_vec_fini_t(allocator, vec, T) \
    ecs_vec_fini(allocator, vec, ECS_SIZEOF(T))

FLECS_API
ecs_vec_t* ecs_vec_reset(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size);

#define ecs_vec_reset_t(allocator, vec, T) \
    ecs_vec_reset(allocator, vec, ECS_SIZEOF(T))

FLECS_API
void ecs_vec_clear(
    ecs_vec_t *vec);

FLECS_API
void* ecs_vec_append(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size);

#define ecs_vec_append_t(allocator, vec, T) \
    ECS_CAST(T*, ecs_vec_append(allocator, vec, ECS_SIZEOF(T)))

FLECS_API
void ecs_vec_remove(
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem);

#define ecs_vec_remove_t(vec, T, elem) \
    ecs_vec_remove(vec, ECS_SIZEOF(T), elem)

FLECS_API
void ecs_vec_remove_last(
    ecs_vec_t *vec);

FLECS_API
ecs_vec_t ecs_vec_copy(
    struct ecs_allocator_t *allocator,
    const ecs_vec_t *vec,
    ecs_size_t size);

#define ecs_vec_copy_t(allocator, vec, T) \
    ecs_vec_copy(allocator, vec, ECS_SIZEOF(T))

FLECS_API
void ecs_vec_reclaim(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size);

#define ecs_vec_reclaim_t(allocator, vec, T) \
    ecs_vec_reclaim(allocator, vec, ECS_SIZEOF(T))

FLECS_API
void ecs_vec_set_size(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem_count);

#define ecs_vec_set_size_t(allocator, vec, T, elem_count) \
    ecs_vec_set_size(allocator, vec, ECS_SIZEOF(T), elem_count)

FLECS_API
void ecs_vec_set_min_size(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem_count);

#define ecs_vec_set_min_size_t(allocator, vec, T, elem_count) \
    ecs_vec_set_min_size(allocator, vec, ECS_SIZEOF(T), elem_count)

FLECS_API
void ecs_vec_set_min_count(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem_count);

#define ecs_vec_set_min_count_t(allocator, vec, T, elem_count) \
    ecs_vec_set_min_count(allocator, vec, ECS_SIZEOF(T), elem_count)

FLECS_API
void ecs_vec_set_min_count_zeromem(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem_count);

#define ecs_vec_set_min_count_zeromem_t(allocator, vec, T, elem_count) \
    ecs_vec_set_min_count_zeromem(allocator, vec, ECS_SIZEOF(T), elem_count)

FLECS_API
void ecs_vec_set_count(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem_count);

#define ecs_vec_set_count_t(allocator, vec, T, elem_count) \
    ecs_vec_set_count(allocator, vec, ECS_SIZEOF(T), elem_count)

FLECS_API
void* ecs_vec_grow(
    struct ecs_allocator_t *allocator,
    ecs_vec_t *vec,
    ecs_size_t size,
    int32_t elem_count);

#define ecs_vec_grow_t(allocator, vec, T, elem_count) \
    ecs_vec_grow(allocator, vec, ECS_SIZEOF(T), elem_count)

FLECS_API
int32_t ecs_vec_count(
    const ecs_vec_t *vec);

FLECS_API
int32_t ecs_vec_size(
    const ecs_vec_t *vec);

FLECS_API
void* ecs_vec_get(
    const ecs_vec_t *vec,
    ecs_size_t size,
    int32_t index);

#define ecs_vec_get_t(vec, T, index) \
    ECS_CAST(T*, ecs_vec_get(vec, ECS_SIZEOF(T), index))

FLECS_API
void* ecs_vec_first(
    const ecs_vec_t *vec);

#define ecs_vec_first_t(vec, T) \
    ECS_CAST(T*, ecs_vec_first(vec))

FLECS_API
void* ecs_vec_last(
    const ecs_vec_t *vec,
    ecs_size_t size);

#define ecs_vec_last_t(vec, T) \
    ECS_CAST(T*, ecs_vec_last(vec, ECS_SIZEOF(T)))


#endif 

/**
 * @file sparse.h
 * @brief Sparse set data structure.
 */

#ifndef FLECS_SPARSE_H
#define FLECS_SPARSE_H



/** The number of elements in a single page */
#define FLECS_SPARSE_PAGE_SIZE (1 << FLECS_SPARSE_PAGE_BITS)

typedef struct ecs_sparse_t {
    ecs_vec_t dense;         /* Dense array with indices to sparse array. The
                              * dense array stores both alive and not alive
                              * sparse indices. The 'count' member keeps
                              * track of which indices are alive. */

    ecs_vec_t pages;         /* Chunks with sparse arrays & data */
    ecs_size_t size;         /* Element size */
    int32_t count;           /* Number of alive entries */
    uint64_t max_id;         /* Local max index (if no global is set) */
    struct ecs_allocator_t *allocator;
    struct ecs_block_allocator_t *page_allocator;
} ecs_sparse_t;

/** Initialize sparse set */
FLECS_DBG_API
void flecs_sparse_init(
    ecs_sparse_t *sparse,
    struct ecs_allocator_t *allocator,
    struct ecs_block_allocator_t *page_allocator,
    ecs_size_t elem_size);

#define flecs_sparse_init_t(sparse, allocator, page_allocator, T)\
    flecs_sparse_init(sparse, allocator, page_allocator, ECS_SIZEOF(T))

FLECS_DBG_API
void flecs_sparse_fini(
    ecs_sparse_t *sparse);

/** Remove all elements from sparse set */
FLECS_DBG_API
void flecs_sparse_clear(
    ecs_sparse_t *sparse);

/** Add element to sparse set, this generates or recycles an id */
FLECS_DBG_API
void* flecs_sparse_add(
    ecs_sparse_t *sparse,
    ecs_size_t elem_size);

#define flecs_sparse_add_t(sparse, T)\
    ECS_CAST(T*, flecs_sparse_add(sparse, ECS_SIZEOF(T)))

/** Get last issued id. */
FLECS_DBG_API
uint64_t flecs_sparse_last_id(
    const ecs_sparse_t *sparse);

/** Generate or recycle a new id. */
FLECS_DBG_API
uint64_t flecs_sparse_new_id(
    ecs_sparse_t *sparse);

/** Remove an element */
FLECS_DBG_API
void flecs_sparse_remove(
    ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    uint64_t id);

#define flecs_sparse_remove_t(sparse, T, id)\
    flecs_sparse_remove(sparse, ECS_SIZEOF(T), id)

/** Test if id is alive, which requires the generation count to match. */
FLECS_DBG_API
bool flecs_sparse_is_alive(
    const ecs_sparse_t *sparse,
    uint64_t id);

/** Get value from sparse set by dense id. This function is useful in 
 * combination with flecs_sparse_count for iterating all values in the set. */
FLECS_DBG_API
void* flecs_sparse_get_dense(
    const ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    int32_t index);

#define flecs_sparse_get_dense_t(sparse, T, index)\
    ECS_CAST(T*, flecs_sparse_get_dense(sparse, ECS_SIZEOF(T), index))

/** Get the number of alive elements in the sparse set. */
FLECS_DBG_API
int32_t flecs_sparse_count(
    const ecs_sparse_t *sparse);

/** Get element by (sparse) id. The returned pointer is stable for the duration
 * of the sparse set, as it is stored in the sparse array. */
FLECS_DBG_API
void* flecs_sparse_get(
    const ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    uint64_t id);

#define flecs_sparse_get_t(sparse, T, index)\
    ECS_CAST(T*, flecs_sparse_get(sparse, ECS_SIZEOF(T), index))

/** Same as flecs_sparse_get, but doesn't assert if id is not alive. */
FLECS_DBG_API
void* flecs_sparse_try(
    const ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    uint64_t id);

#define flecs_sparse_try_t(sparse, T, index)\
    ECS_CAST(T*, flecs_sparse_try(sparse, ECS_SIZEOF(T), index))

/** Like get_sparse, but don't care whether element is alive or not. */
FLECS_DBG_API
void* flecs_sparse_get_any(
    const ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    uint64_t id);

#define flecs_sparse_get_any_t(sparse, T, index)\
    ECS_CAST(T*, flecs_sparse_get_any(sparse, ECS_SIZEOF(T), index))

/** Get or create element by (sparse) id. */
FLECS_DBG_API
void* flecs_sparse_ensure(
    ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    uint64_t id);

#define flecs_sparse_ensure_t(sparse, T, index)\
    ECS_CAST(T*, flecs_sparse_ensure(sparse, ECS_SIZEOF(T), index))

/** Fast version of ensure, no liveliness checking */
FLECS_DBG_API
void* flecs_sparse_ensure_fast(
    ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    uint64_t id);

#define flecs_sparse_ensure_fast_t(sparse, T, index)\
    ECS_CAST(T*, flecs_sparse_ensure_fast(sparse, ECS_SIZEOF(T), index))

/** Get pointer to ids (alive and not alive). Use with count() or size(). */
FLECS_DBG_API
const uint64_t* flecs_sparse_ids(
    const ecs_sparse_t *sparse);

/* Publicly exposed APIs 
 * The flecs_ functions aren't exposed directly as this can cause some
 * optimizers to not consider them for link time optimization. */

FLECS_API
void ecs_sparse_init(
    ecs_sparse_t *sparse,
    ecs_size_t elem_size);

#define ecs_sparse_init_t(sparse, T)\
    ecs_sparse_init(sparse, ECS_SIZEOF(T))

FLECS_API
void* ecs_sparse_add(
    ecs_sparse_t *sparse,
    ecs_size_t elem_size);

#define ecs_sparse_add_t(sparse, T)\
    ECS_CAST(T*, ecs_sparse_add(sparse, ECS_SIZEOF(T)))

FLECS_API
uint64_t ecs_sparse_last_id(
    const ecs_sparse_t *sparse);

FLECS_API
int32_t ecs_sparse_count(
    const ecs_sparse_t *sparse);

/** Override the generation count for a specific id */
FLECS_API
void flecs_sparse_set_generation(
    ecs_sparse_t *sparse,
    uint64_t id);

FLECS_API
void* ecs_sparse_get_dense(
    const ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    int32_t index);

#define ecs_sparse_get_dense_t(sparse, T, index)\
    ECS_CAST(T*, ecs_sparse_get_dense(sparse, ECS_SIZEOF(T), index))

FLECS_API
void* ecs_sparse_get(
    const ecs_sparse_t *sparse,
    ecs_size_t elem_size,
    uint64_t id);

#define ecs_sparse_get_t(sparse, T, index)\
    ECS_CAST(T*, ecs_sparse_get(sparse, ECS_SIZEOF(T), index))


#endif

/**
 * @file block_allocator.h
 * @brief Block allocator.
 */

#ifndef FLECS_BLOCK_ALLOCATOR_H
#define FLECS_BLOCK_ALLOCATOR_H


typedef struct ecs_block_allocator_block_t {
    void *memory;
    struct ecs_block_allocator_block_t *next;
} ecs_block_allocator_block_t;

typedef struct ecs_block_allocator_chunk_header_t {
    struct ecs_block_allocator_chunk_header_t *next;
} ecs_block_allocator_chunk_header_t;

typedef struct ecs_block_allocator_t {
    ecs_block_allocator_chunk_header_t *head;
    ecs_block_allocator_block_t *block_head;
    ecs_block_allocator_block_t *block_tail;
    int32_t chunk_size;
    int32_t data_size;
    int32_t chunks_per_block;
    int32_t block_size;
    int32_t alloc_count;
} ecs_block_allocator_t;

FLECS_API
void flecs_ballocator_init(
    ecs_block_allocator_t *ba,
    ecs_size_t size);

#define flecs_ballocator_init_t(ba, T)\
    flecs_ballocator_init(ba, ECS_SIZEOF(T))
#define flecs_ballocator_init_n(ba, T, count)\
    flecs_ballocator_init(ba, ECS_SIZEOF(T) * count)

FLECS_API
ecs_block_allocator_t* flecs_ballocator_new(
    ecs_size_t size);

#define flecs_ballocator_new_t(T)\
    flecs_ballocator_new(ECS_SIZEOF(T))
#define flecs_ballocator_new_n(T, count)\
    flecs_ballocator_new(ECS_SIZEOF(T) * count)

FLECS_API
void flecs_ballocator_fini(
    ecs_block_allocator_t *ba);

FLECS_API
void flecs_ballocator_free(
    ecs_block_allocator_t *ba);

FLECS_API
void* flecs_balloc(
    ecs_block_allocator_t *allocator);

FLECS_API
void* flecs_bcalloc(
    ecs_block_allocator_t *allocator);

FLECS_API
void flecs_bfree(
    ecs_block_allocator_t *allocator, 
    void *memory);

FLECS_API
void* flecs_brealloc(
    ecs_block_allocator_t *dst, 
    ecs_block_allocator_t *src, 
    void *memory);

FLECS_API
void* flecs_bdup(
    ecs_block_allocator_t *ba, 
    void *memory);

#endif

/**
 * @file map.h
 * @brief Map data structure.
 */

#ifndef FLECS_MAP_H
#define FLECS_MAP_H



typedef uint64_t ecs_map_data_t;
typedef ecs_map_data_t ecs_map_key_t;
typedef ecs_map_data_t ecs_map_val_t;

/* Map type */
typedef struct ecs_bucket_entry_t {
    ecs_map_key_t key;
    ecs_map_val_t value;
    struct ecs_bucket_entry_t *next;
} ecs_bucket_entry_t;

typedef struct ecs_bucket_t {
    ecs_bucket_entry_t *first;
} ecs_bucket_t;

typedef struct ecs_map_t {
    uint8_t bucket_shift;
    bool shared_allocator;
    ecs_bucket_t *buckets;
    int32_t bucket_count;
    int32_t count;
    struct ecs_block_allocator_t *entry_allocator;
    struct ecs_allocator_t *allocator;
} ecs_map_t;

typedef struct ecs_map_iter_t {
    const ecs_map_t *map;
    ecs_bucket_t *bucket;
    ecs_bucket_entry_t *entry;
    ecs_map_data_t *res;
} ecs_map_iter_t;

typedef struct ecs_map_params_t {
    struct ecs_allocator_t *allocator;
    struct ecs_block_allocator_t entry_allocator;
} ecs_map_params_t;

/* Function/macro postfixes meaning:
 *   _ptr:    access ecs_map_val_t as void*
 *   _ref:    access ecs_map_val_t* as T**
 *   _deref:  dereferences a _ref
 *   _alloc:  if _ptr is NULL, alloc
 *   _free:   if _ptr is not NULL, free
 */

FLECS_API
void ecs_map_params_init(
    ecs_map_params_t *params,
    struct ecs_allocator_t *allocator);

FLECS_API
void ecs_map_params_fini(
    ecs_map_params_t *params);

/** Initialize new map. */
FLECS_API
void ecs_map_init(
    ecs_map_t *map,
    struct ecs_allocator_t *allocator);

/** Initialize new map. */
FLECS_API
void ecs_map_init_w_params(
    ecs_map_t *map,
    ecs_map_params_t *params);

/** Initialize new map if uninitialized, leave as is otherwise */
FLECS_API
void ecs_map_init_if(
    ecs_map_t *map,
    struct ecs_allocator_t *allocator);

FLECS_API
void ecs_map_init_w_params_if(
    ecs_map_t *result,
    ecs_map_params_t *params);

/** Deinitialize map. */
FLECS_API
void ecs_map_fini(
    ecs_map_t *map);

/** Get element for key, returns NULL if they key doesn't exist. */
FLECS_API
ecs_map_val_t* ecs_map_get(
    const ecs_map_t *map,
    ecs_map_key_t key);

/* Get element as pointer (auto-dereferences _ptr) */
FLECS_API
void* ecs_map_get_deref_(
    const ecs_map_t *map,
    ecs_map_key_t key);

/** Get or insert element for key. */
FLECS_API
ecs_map_val_t* ecs_map_ensure(
    ecs_map_t *map,
    ecs_map_key_t key);

/** Get or insert pointer element for key, allocate if the pointer is NULL */
FLECS_API
void* ecs_map_ensure_alloc(
    ecs_map_t *map,
    ecs_size_t elem_size,
    ecs_map_key_t key);

/** Insert element for key. */
FLECS_API
void ecs_map_insert(
    ecs_map_t *map,
    ecs_map_key_t key,
    ecs_map_val_t value);

/** Insert pointer element for key, populate with new allocation. */
FLECS_API
void* ecs_map_insert_alloc(
    ecs_map_t *map,
    ecs_size_t elem_size,
    ecs_map_key_t key);

/** Remove key from map. */
FLECS_API
ecs_map_val_t ecs_map_remove(
    ecs_map_t *map,
    ecs_map_key_t key);

/* Remove pointer element, free if not NULL */
FLECS_API
void ecs_map_remove_free(
    ecs_map_t *map,
    ecs_map_key_t key);

/** Remove all elements from map. */
FLECS_API
void ecs_map_clear(
    ecs_map_t *map);

/** Return number of elements in map. */
#define ecs_map_count(map) ((map) ? (map)->count : 0)

/** Is map initialized */
#define ecs_map_is_init(map) ((map) ? (map)->bucket_shift != 0 : false)

/** Return iterator to map contents. */
FLECS_API
ecs_map_iter_t ecs_map_iter(
    const ecs_map_t *map);

/** Obtain next element in map from iterator. */
FLECS_API
bool ecs_map_next(
    ecs_map_iter_t *iter);

/** Copy map. */
FLECS_API
void ecs_map_copy(
    ecs_map_t *dst,
    const ecs_map_t *src);

#define ecs_map_get_ref(m, T, k) ECS_CAST(T**, ecs_map_get(m, k))
#define ecs_map_get_deref(m, T, k) ECS_CAST(T*, ecs_map_get_deref_(m, k))
#define ecs_map_ensure_ref(m, T, k) ECS_CAST(T**, ecs_map_ensure(m, k))

#define ecs_map_insert_ptr(m, k, v) ecs_map_insert(m, k, ECS_CAST(ecs_map_val_t, ECS_PTR_CAST(uintptr_t, v)))
#define ecs_map_insert_alloc_t(m, T, k) ECS_CAST(T*, ecs_map_insert_alloc(m, ECS_SIZEOF(T), k))
#define ecs_map_ensure_alloc_t(m, T, k) ECS_PTR_CAST(T*, (uintptr_t)ecs_map_ensure_alloc(m, ECS_SIZEOF(T), k))
#define ecs_map_remove_ptr(m, k) (ECS_PTR_CAST(void*, ECS_CAST(uintptr_t, (ecs_map_remove(m, k)))))

#define ecs_map_key(it) ((it)->res[0])
#define ecs_map_value(it) ((it)->res[1])
#define ecs_map_ptr(it) ECS_PTR_CAST(void*, ECS_CAST(uintptr_t, ecs_map_value(it)))
#define ecs_map_ref(it, T) (ECS_CAST(T**, &((it)->res[1])))


#endif

/**
 * @file allocator.h
 * @brief Allocator that returns memory objects of any size. 
 */

#ifndef FLECS_ALLOCATOR_H
#define FLECS_ALLOCATOR_H


FLECS_DBG_API extern int64_t ecs_block_allocator_alloc_count;
FLECS_DBG_API extern int64_t ecs_block_allocator_free_count;
FLECS_DBG_API extern int64_t ecs_stack_allocator_alloc_count;
FLECS_DBG_API extern int64_t ecs_stack_allocator_free_count;

struct ecs_allocator_t {
    ecs_block_allocator_t chunks;
    struct ecs_sparse_t sizes; /* <size, block_allocator_t> */
};

FLECS_API
void flecs_allocator_init(
    ecs_allocator_t *a);

FLECS_API
void flecs_allocator_fini(
    ecs_allocator_t *a);

FLECS_API
ecs_block_allocator_t* flecs_allocator_get(
    ecs_allocator_t *a, 
    ecs_size_t size);

FLECS_API
char* flecs_strdup(
    ecs_allocator_t *a, 
    const char* str);

FLECS_API
void flecs_strfree(
    ecs_allocator_t *a, 
    char* str);

FLECS_API
void* flecs_dup(
    ecs_allocator_t *a,
    ecs_size_t size,
    const void *src);

#define flecs_allocator(obj) (&obj->allocators.dyn)

#define flecs_alloc(a, size) flecs_balloc(flecs_allocator_get(a, size))
#define flecs_alloc_t(a, T) flecs_alloc(a, ECS_SIZEOF(T))
#define flecs_alloc_n(a, T, count) flecs_alloc(a, ECS_SIZEOF(T) * (count))

#define flecs_calloc(a, size) flecs_bcalloc(flecs_allocator_get(a, size))
#define flecs_calloc_t(a, T) flecs_calloc(a, ECS_SIZEOF(T))
#define flecs_calloc_n(a, T, count) flecs_calloc(a, ECS_SIZEOF(T) * (count))

#define flecs_free(a, size, ptr) flecs_bfree(flecs_allocator_get(a, size), ptr)
#define flecs_free_t(a, T, ptr) flecs_free(a, ECS_SIZEOF(T), ptr)
#define flecs_free_n(a, T, count, ptr) flecs_free(a, ECS_SIZEOF(T) * (count), ptr)

#define flecs_realloc(a, size_dst, size_src, ptr)\
    flecs_brealloc(flecs_allocator_get(a, size_dst),\
    flecs_allocator_get(a, size_src),\
    ptr)
#define flecs_realloc_n(a, T, count_dst, count_src, ptr)\
    flecs_realloc(a, ECS_SIZEOF(T) * (count_dst), ECS_SIZEOF(T) * (count_src), ptr)

#define flecs_dup_n(a, T, count, ptr) flecs_dup(a, ECS_SIZEOF(T) * (count), ptr)

#endif

/**
 * @file strbuf.h
 * @brief Utility for constructing strings.
 */

#ifndef FLECS_STRBUF_H_
#define FLECS_STRBUF_H_



#define ECS_STRBUF_INIT (ecs_strbuf_t){0}
#define ECS_STRBUF_ELEMENT_SIZE (511)
#define ECS_STRBUF_MAX_LIST_DEPTH (32)

typedef struct ecs_strbuf_element {
    bool buffer_embedded;
    int32_t pos;
    char *buf;
    struct ecs_strbuf_element *next;
} ecs_strbuf_element;

typedef struct ecs_strbuf_element_embedded {
    ecs_strbuf_element super;
    char buf[ECS_STRBUF_ELEMENT_SIZE + 1];
} ecs_strbuf_element_embedded;

typedef struct ecs_strbuf_element_str {
    ecs_strbuf_element super;
    char *alloc_str;
} ecs_strbuf_element_str;

typedef struct ecs_strbuf_list_elem {
    int32_t count;
    const char *separator;
} ecs_strbuf_list_elem;

typedef struct ecs_strbuf_t {
    /* When set by an application, append will write to this buffer */
    char *buf;

    /* The maximum number of characters that may be printed */
    int32_t max;

    /* Size of elements minus current element */
    int32_t size;

    /* The number of elements in use */
    int32_t elementCount;

    /* Always allocate at least one element */
    ecs_strbuf_element_embedded firstElement;

    /* The current element being appended to */
    ecs_strbuf_element *current;

    /* Stack that keeps track of number of list elements, used for conditionally
     * inserting a separator */
    ecs_strbuf_list_elem list_stack[ECS_STRBUF_MAX_LIST_DEPTH];
    int32_t list_sp;

    /* This is set to the output string after calling ecs_strbuf_get */
    char *content;

    /* This is set to the output string length after calling ecs_strbuf_get */
    int32_t length;
} ecs_strbuf_t;

/* Append format string to a buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_append(
    ecs_strbuf_t *buffer,
    const char *fmt,
    ...);

/* Append format string with argument list to a buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_vappend(
    ecs_strbuf_t *buffer,
    const char *fmt,
    va_list args);

/* Append string to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendstr(
    ecs_strbuf_t *buffer,
    const char *str);

/* Append character to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendch(
    ecs_strbuf_t *buffer,
    char ch);

/* Append int to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendint(
    ecs_strbuf_t *buffer,
    int64_t v);

/* Append float to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendflt(
    ecs_strbuf_t *buffer,
    double v,
    char nan_delim);

/* Append boolean to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendbool(
    ecs_strbuf_t *buffer,
    bool v);

/* Append source buffer to destination buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_mergebuff(
    ecs_strbuf_t *dst_buffer,
    ecs_strbuf_t *src_buffer);

/* Append string to buffer, transfer ownership to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendstr_zerocpy(
    ecs_strbuf_t *buffer,
    char *str);

/* Append string to buffer, transfer ownership to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendstr_zerocpyn(
    ecs_strbuf_t *buffer,
    char *str,
    int32_t n);

/* Append string to buffer, do not free/modify string.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendstr_zerocpy_const(
    ecs_strbuf_t *buffer,
    const char *str);

/* Append string to buffer, transfer ownership to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendstr_zerocpyn_const(
    ecs_strbuf_t *buffer,
    const char *str,
    int32_t n);

/* Append n characters to buffer.
 * Returns false when max is reached, true when there is still space */
FLECS_API
bool ecs_strbuf_appendstrn(
    ecs_strbuf_t *buffer,
    const char *str,
    int32_t n);

/* Return result string */
FLECS_API
char *ecs_strbuf_get(
    ecs_strbuf_t *buffer);

/* Return small string from first element (appends \0) */
FLECS_API
char *ecs_strbuf_get_small(
    ecs_strbuf_t *buffer);

/* Reset buffer without returning a string */
FLECS_API
void ecs_strbuf_reset(
    ecs_strbuf_t *buffer);

/* Push a list */
FLECS_API
void ecs_strbuf_list_push(
    ecs_strbuf_t *buffer,
    const char *list_open,
    const char *separator);

/* Pop a new list */
FLECS_API
void ecs_strbuf_list_pop(
    ecs_strbuf_t *buffer,
    const char *list_close);

/* Insert a new element in list */
FLECS_API
void ecs_strbuf_list_next(
    ecs_strbuf_t *buffer);

/* Append character to as new element in list. */
FLECS_API
bool ecs_strbuf_list_appendch(
    ecs_strbuf_t *buffer,
    char ch);

/* Append formatted string as a new element in list */
FLECS_API
bool ecs_strbuf_list_append(
    ecs_strbuf_t *buffer,
    const char *fmt,
    ...);

/* Append string as a new element in list */
FLECS_API
bool ecs_strbuf_list_appendstr(
    ecs_strbuf_t *buffer,
    const char *str);

/* Append string as a new element in list */
FLECS_API
bool ecs_strbuf_list_appendstrn(
    ecs_strbuf_t *buffer,
    const char *str,
    int32_t n);

FLECS_API
int32_t ecs_strbuf_written(
    const ecs_strbuf_t *buffer);

#define ecs_strbuf_appendlit(buf, str)\
    ecs_strbuf_appendstrn(buf, str, (int32_t)(sizeof(str) - 1))

#define ecs_strbuf_list_appendlit(buf, str)\
    ecs_strbuf_list_appendstrn(buf, str, (int32_t)(sizeof(str) - 1))


#endif

/**
 * @file os_api.h
 * @brief Operating system abstraction API.
 *
 * This file contains the operating system abstraction API. The flecs core 
 * library avoids OS/runtime specific API calls as much as possible. Instead it
 * provides an interface that can be implemented by applications.
 *
 * Examples for how to implement this interface can be found in the 
 * examples/os_api folder.
 */

#ifndef FLECS_OS_API_H
#define FLECS_OS_API_H

/**
 * @defgroup c_os_api OS API
 * @brief Interface for providing OS specific functionality.
 * 
 * \ingroup c
 * @{
 */

#include <stdarg.h>
#include <errno.h>

#if defined(ECS_TARGET_WINDOWS)
#include <malloc.h>
#elif defined(ECS_TARGET_FREEBSD)
#include <stdlib.h>
#else
#include <alloca.h>
#endif


typedef struct ecs_time_t {
    uint32_t sec;
    uint32_t nanosec;
} ecs_time_t;

/* Allocation counters */
extern int64_t ecs_os_api_malloc_count;
extern int64_t ecs_os_api_realloc_count;
extern int64_t ecs_os_api_calloc_count;
extern int64_t ecs_os_api_free_count;

/* Use handle types that _at least_ can store pointers */
typedef uintptr_t ecs_os_thread_t;
typedef uintptr_t ecs_os_cond_t;
typedef uintptr_t ecs_os_mutex_t;
typedef uintptr_t ecs_os_dl_t;
typedef uintptr_t ecs_os_sock_t;

/* 64 bit thread id */
typedef uint64_t ecs_os_thread_id_t;

/* Generic function pointer type */
typedef void (*ecs_os_proc_t)(void);

/* OS API init */
typedef 
void (*ecs_os_api_init_t)(void);

/* OS API deinit */
typedef 
void (*ecs_os_api_fini_t)(void);

/* Memory management */
typedef 
void* (*ecs_os_api_malloc_t)(
    ecs_size_t size);

typedef 
void (*ecs_os_api_free_t)(
    void *ptr);

typedef
void* (*ecs_os_api_realloc_t)(
    void *ptr, 
    ecs_size_t size);

typedef
void* (*ecs_os_api_calloc_t)(
    ecs_size_t size);

typedef
char* (*ecs_os_api_strdup_t)(
    const char *str);

/* Threads */
typedef
void* (*ecs_os_thread_callback_t)(
    void*);

typedef
ecs_os_thread_t (*ecs_os_api_thread_new_t)(
    ecs_os_thread_callback_t callback,
    void *param);

typedef
void* (*ecs_os_api_thread_join_t)(
    ecs_os_thread_t thread);

typedef
ecs_os_thread_id_t (*ecs_os_api_thread_self_t)(void);

/* Tasks */
typedef
ecs_os_thread_t (*ecs_os_api_task_new_t)(
    ecs_os_thread_callback_t callback,
    void *param);

typedef
void* (*ecs_os_api_task_join_t)(
    ecs_os_thread_t thread);

/* Atomic increment / decrement */
typedef
int32_t (*ecs_os_api_ainc_t)(
    int32_t *value);

typedef
int64_t (*ecs_os_api_lainc_t)(
    int64_t *value);

/* Mutex */
typedef
ecs_os_mutex_t (*ecs_os_api_mutex_new_t)(
    void);

typedef
void (*ecs_os_api_mutex_lock_t)(
    ecs_os_mutex_t mutex);

typedef
void (*ecs_os_api_mutex_unlock_t)(
    ecs_os_mutex_t mutex);

typedef
void (*ecs_os_api_mutex_free_t)(
    ecs_os_mutex_t mutex);

/* Condition variable */
typedef
ecs_os_cond_t (*ecs_os_api_cond_new_t)(
    void);

typedef
void (*ecs_os_api_cond_free_t)(
    ecs_os_cond_t cond);

typedef
void (*ecs_os_api_cond_signal_t)(
    ecs_os_cond_t cond);

typedef
void (*ecs_os_api_cond_broadcast_t)(
    ecs_os_cond_t cond);

typedef
void (*ecs_os_api_cond_wait_t)(
    ecs_os_cond_t cond,
    ecs_os_mutex_t mutex);

typedef 
void (*ecs_os_api_sleep_t)(
    int32_t sec,
    int32_t nanosec);

typedef 
void (*ecs_os_api_enable_high_timer_resolution_t)(
    bool enable);

typedef
void (*ecs_os_api_get_time_t)(
    ecs_time_t *time_out);

typedef
uint64_t (*ecs_os_api_now_t)(void);

/* Logging */
typedef
void (*ecs_os_api_log_t)(
    int32_t level,     /* Logging level */
    const char *file,  /* File where message was logged */
    int32_t line,      /* Line it was logged */
    const char *msg);

/* Application termination */
typedef
void (*ecs_os_api_abort_t)(
    void);

/* Dynamic libraries */
typedef
ecs_os_dl_t (*ecs_os_api_dlopen_t)(
    const char *libname);

typedef
ecs_os_proc_t (*ecs_os_api_dlproc_t)(
    ecs_os_dl_t lib,
    const char *procname);

typedef
void (*ecs_os_api_dlclose_t)(
    ecs_os_dl_t lib);

typedef
char* (*ecs_os_api_module_to_path_t)(
    const char *module_id);

/* Prefix members of struct with 'ecs_' as some system headers may define 
 * macros for functions like "strdup", "log" or "_free" */

typedef struct ecs_os_api_t {
    /* API init / deinit */
    ecs_os_api_init_t init_;
    ecs_os_api_fini_t fini_;

    /* Memory management */
    ecs_os_api_malloc_t malloc_;
    ecs_os_api_realloc_t realloc_;
    ecs_os_api_calloc_t calloc_;
    ecs_os_api_free_t free_;

    /* Strings */
    ecs_os_api_strdup_t strdup_;

    /* Threads */
    ecs_os_api_thread_new_t thread_new_;
    ecs_os_api_thread_join_t thread_join_;
    ecs_os_api_thread_self_t thread_self_;

    /* Tasks */
    ecs_os_api_thread_new_t task_new_;
    ecs_os_api_thread_join_t task_join_;

    /* Atomic increment / decrement */
    ecs_os_api_ainc_t ainc_;
    ecs_os_api_ainc_t adec_;
    ecs_os_api_lainc_t lainc_;
    ecs_os_api_lainc_t ladec_;

    /* Mutex */
    ecs_os_api_mutex_new_t mutex_new_;
    ecs_os_api_mutex_free_t mutex_free_;
    ecs_os_api_mutex_lock_t mutex_lock_;
    ecs_os_api_mutex_lock_t mutex_unlock_;

    /* Condition variable */
    ecs_os_api_cond_new_t cond_new_;
    ecs_os_api_cond_free_t cond_free_;
    ecs_os_api_cond_signal_t cond_signal_;
    ecs_os_api_cond_broadcast_t cond_broadcast_;
    ecs_os_api_cond_wait_t cond_wait_;

    /* Time */
    ecs_os_api_sleep_t sleep_;
    ecs_os_api_now_t now_;
    ecs_os_api_get_time_t get_time_;

    /* Logging */
    ecs_os_api_log_t log_; /* Logging function. The level should be interpreted as: */
                           /* >0: Debug tracing. Only enabled in debug builds. */
                           /*  0: Tracing. Enabled in debug/release builds. */
                           /* -2: Warning. An issue occurred, but operation was successful. */
                           /* -3: Error. An issue occurred, and operation was unsuccessful. */
                           /* -4: Fatal. An issue occurred, and application must quit. */

    /* Application termination */
    ecs_os_api_abort_t abort_;

    /* Dynamic library loading */
    ecs_os_api_dlopen_t dlopen_;
    ecs_os_api_dlproc_t dlproc_;
    ecs_os_api_dlclose_t dlclose_;

    /* Overridable function that translates from a logical module id to a
     * shared library filename */
    ecs_os_api_module_to_path_t module_to_dl_;

    /* Overridable function that translates from a logical module id to a
     * path that contains module-specif resources or assets */
    ecs_os_api_module_to_path_t module_to_etc_;

    /* Trace level */
    int32_t log_level_;

    /* Trace indentation */
    int32_t log_indent_;

    /* Last error code */
    int32_t log_last_error_;

    /* Last recorded timestamp */
    int64_t log_last_timestamp_;

    /* OS API flags */
    ecs_flags32_t flags_;
} ecs_os_api_t;

FLECS_API
extern ecs_os_api_t ecs_os_api;

FLECS_API
void ecs_os_init(void);

FLECS_API
void ecs_os_fini(void);

FLECS_API
void ecs_os_set_api(
    ecs_os_api_t *os_api);

FLECS_API
ecs_os_api_t ecs_os_get_api(void);

FLECS_API
void ecs_os_set_api_defaults(void);

/* Memory management */
#ifndef ecs_os_malloc
#define ecs_os_malloc(size) ecs_os_api.malloc_(size)
#endif
#ifndef ecs_os_free
#define ecs_os_free(ptr) ecs_os_api.free_(ptr)
#endif
#ifndef ecs_os_realloc
#define ecs_os_realloc(ptr, size) ecs_os_api.realloc_(ptr, size)
#endif
#ifndef ecs_os_calloc
#define ecs_os_calloc(size) ecs_os_api.calloc_(size)
#endif
#if defined(ECS_TARGET_WINDOWS)
#define ecs_os_alloca(size) _alloca((size_t)(size))
#else
#define ecs_os_alloca(size) alloca((size_t)(size))
#endif

#define ecs_os_malloc_t(T) ECS_CAST(T*, ecs_os_malloc(ECS_SIZEOF(T)))
#define ecs_os_malloc_n(T, count) ECS_CAST(T*, ecs_os_malloc(ECS_SIZEOF(T) * (count)))
#define ecs_os_calloc_t(T) ECS_CAST(T*, ecs_os_calloc(ECS_SIZEOF(T)))
#define ecs_os_calloc_n(T, count) ECS_CAST(T*, ecs_os_calloc(ECS_SIZEOF(T) * (count)))

#define ecs_os_realloc_t(ptr, T) ECS_CAST(T*, ecs_os_realloc(ptr, ECS_SIZEOF(T)))
#define ecs_os_realloc_n(ptr, T, count) ECS_CAST(T*, ecs_os_realloc(ptr, ECS_SIZEOF(T) * (count)))
#define ecs_os_alloca_t(T) ECS_CAST(T*, ecs_os_alloca(ECS_SIZEOF(T)))
#define ecs_os_alloca_n(T, count) ECS_CAST(T*, ecs_os_alloca(ECS_SIZEOF(T) * (count)))

/* Strings */
#ifndef ecs_os_strdup
#define ecs_os_strdup(str) ecs_os_api.strdup_(str)
#endif

#define ecs_os_strlen(str) (ecs_size_t)strlen(str)
#define ecs_os_strncmp(str1, str2, num) strncmp(str1, str2, (size_t)(num))
#define ecs_os_memcmp(ptr1, ptr2, num) memcmp(ptr1, ptr2, (size_t)(num))
#define ecs_os_memcpy(ptr1, ptr2, num) memcpy(ptr1, ptr2, (size_t)(num))
#define ecs_os_memset(ptr, value, num) memset(ptr, value, (size_t)(num))
#define ecs_os_memmove(dst, src, size) memmove(dst, src, (size_t)(size))

#define ecs_os_memcpy_t(ptr1, ptr2, T) ecs_os_memcpy(ptr1, ptr2, ECS_SIZEOF(T))
#define ecs_os_memcpy_n(ptr1, ptr2, T, count) ecs_os_memcpy(ptr1, ptr2, ECS_SIZEOF(T) * count)
#define ecs_os_memcmp_t(ptr1, ptr2, T) ecs_os_memcmp(ptr1, ptr2, ECS_SIZEOF(T))

#define ecs_os_memmove_t(ptr1, ptr2, T) ecs_os_memmove(ptr1, ptr2, ECS_SIZEOF(T))
#define ecs_os_memmove_n(ptr1, ptr2, T, count) ecs_os_memmove(ptr1, ptr2, ECS_SIZEOF(T) * count)
#define ecs_os_memmove_t(ptr1, ptr2, T) ecs_os_memmove(ptr1, ptr2, ECS_SIZEOF(T))

#define ecs_os_strcmp(str1, str2) strcmp(str1, str2)
#define ecs_os_memset_t(ptr, value, T) ecs_os_memset(ptr, value, ECS_SIZEOF(T))
#define ecs_os_memset_n(ptr, value, T, count) ecs_os_memset(ptr, value, ECS_SIZEOF(T) * count)
#define ecs_os_zeromem(ptr) ecs_os_memset(ptr, 0, ECS_SIZEOF(*ptr))

#define ecs_os_memdup_t(ptr, T) ecs_os_memdup(ptr, ECS_SIZEOF(T))
#define ecs_os_memdup_n(ptr, T, count) ecs_os_memdup(ptr, ECS_SIZEOF(T) * count)

#define ecs_offset(ptr, T, index)\
    ECS_CAST(T*, ECS_OFFSET(ptr, ECS_SIZEOF(T) * index))

#if !defined(ECS_TARGET_POSIX) && !defined(ECS_TARGET_MINGW)
#define ecs_os_strcat(str1, str2) strcat_s(str1, INT_MAX, str2)
#define ecs_os_sprintf(ptr, ...) sprintf_s(ptr, INT_MAX, __VA_ARGS__)
#define ecs_os_vsprintf(ptr, fmt, args) vsprintf_s(ptr, INT_MAX, fmt, args)
#define ecs_os_strcpy(str1, str2) strcpy_s(str1, INT_MAX, str2)
#define ecs_os_strncpy(str1, str2, num) strncpy_s(str1, INT_MAX, str2, (size_t)(num))
#else
#define ecs_os_strcat(str1, str2) strcat(str1, str2)
#define ecs_os_sprintf(ptr, ...) sprintf(ptr, __VA_ARGS__)
#define ecs_os_vsprintf(ptr, fmt, args) vsprintf(ptr, fmt, args)
#define ecs_os_strcpy(str1, str2) strcpy(str1, str2)
#define ecs_os_strncpy(str1, str2, num) strncpy(str1, str2, (size_t)(num))
#endif

/* Files */
#ifndef ECS_TARGET_POSIX
#define ecs_os_fopen(result, file, mode) fopen_s(result, file, mode)
#else
#define ecs_os_fopen(result, file, mode) (*(result)) = fopen(file, mode)
#endif

/* Threads */
#define ecs_os_thread_new(callback, param) ecs_os_api.thread_new_(callback, param)
#define ecs_os_thread_join(thread) ecs_os_api.thread_join_(thread)
#define ecs_os_thread_self() ecs_os_api.thread_self_()

/* Tasks */
#define ecs_os_task_new(callback, param) ecs_os_api.task_new_(callback, param)
#define ecs_os_task_join(thread) ecs_os_api.task_join_(thread)

/* Atomic increment / decrement */
#define ecs_os_ainc(value) ecs_os_api.ainc_(value)
#define ecs_os_adec(value) ecs_os_api.adec_(value)
#define ecs_os_lainc(value) ecs_os_api.lainc_(value)
#define ecs_os_ladec(value) ecs_os_api.ladec_(value)

/* Mutex */
#define ecs_os_mutex_new() ecs_os_api.mutex_new_()
#define ecs_os_mutex_free(mutex) ecs_os_api.mutex_free_(mutex)
#define ecs_os_mutex_lock(mutex) ecs_os_api.mutex_lock_(mutex)
#define ecs_os_mutex_unlock(mutex) ecs_os_api.mutex_unlock_(mutex)

/* Condition variable */
#define ecs_os_cond_new() ecs_os_api.cond_new_()
#define ecs_os_cond_free(cond) ecs_os_api.cond_free_(cond)
#define ecs_os_cond_signal(cond) ecs_os_api.cond_signal_(cond)
#define ecs_os_cond_broadcast(cond) ecs_os_api.cond_broadcast_(cond)
#define ecs_os_cond_wait(cond, mutex) ecs_os_api.cond_wait_(cond, mutex)

/* Time */
#define ecs_os_sleep(sec, nanosec) ecs_os_api.sleep_(sec, nanosec)
#define ecs_os_now() ecs_os_api.now_()
#define ecs_os_get_time(time_out) ecs_os_api.get_time_(time_out)

/* Logging */
FLECS_API
void ecs_os_dbg(const char *file, int32_t line, const char *msg);

FLECS_API
void ecs_os_trace(const char *file, int32_t line, const char *msg);

FLECS_API
void ecs_os_warn(const char *file, int32_t line, const char *msg);

FLECS_API
void ecs_os_err(const char *file, int32_t line, const char *msg);

FLECS_API
void ecs_os_fatal(const char *file, int32_t line, const char *msg);

FLECS_API
const char* ecs_os_strerror(int err);

FLECS_API
void ecs_os_strset(char **str, const char *value);

#ifdef FLECS_ACCURATE_COUNTERS
#define ecs_os_inc(v)  (ecs_os_ainc(v))
#define ecs_os_linc(v) (ecs_os_lainc(v))
#define ecs_os_dec(v)  (ecs_os_adec(v))
#define ecs_os_ldec(v) (ecs_os_ladec(v))
#else
#define ecs_os_inc(v)  (++(*v))
#define ecs_os_linc(v) (++(*v))
#define ecs_os_dec(v)  (--(*v))
#define ecs_os_ldec(v) (--(*v))
#endif

#ifdef ECS_TARGET_MINGW
/* mingw bug: without this a conversion error is thrown, but isnan/isinf should
 * accept float, double and long double. */
#define ecs_os_isnan(val) (isnan((float)val))
#define ecs_os_isinf(val) (isinf((float)val))
#else
#define ecs_os_isnan(val) (isnan(val))
#define ecs_os_isinf(val) (isinf(val))
#endif

/* Application termination */
#define ecs_os_abort() ecs_os_api.abort_()

/* Dynamic libraries */
#define ecs_os_dlopen(libname) ecs_os_api.dlopen_(libname)
#define ecs_os_dlproc(lib, procname) ecs_os_api.dlproc_(lib, procname)
#define ecs_os_dlclose(lib) ecs_os_api.dlclose_(lib)

/* Module id translation */
#define ecs_os_module_to_dl(lib) ecs_os_api.module_to_dl_(lib)
#define ecs_os_module_to_etc(lib) ecs_os_api.module_to_etc_(lib)

/* Sleep with floating point time */
FLECS_API
void ecs_sleepf(
    double t);

/* Measure time since provided timestamp */
FLECS_API
double ecs_time_measure(
    ecs_time_t *start);

/* Calculate difference between two timestamps */
FLECS_API
ecs_time_t ecs_time_sub(
    ecs_time_t t1,
    ecs_time_t t2);

/* Convert time value to a double */
FLECS_API
double ecs_time_to_double(
    ecs_time_t t);

FLECS_API
void* ecs_os_memdup(
    const void *src, 
    ecs_size_t size);

/** Are heap functions available? */
FLECS_API
bool ecs_os_has_heap(void);

/** Are threading functions available? */
FLECS_API
bool ecs_os_has_threading(void);

/** Are task functions available? */
FLECS_API
bool ecs_os_has_task_support(void);

/** Are time functions available? */
FLECS_API
bool ecs_os_has_time(void);

/** Are logging functions available? */
FLECS_API
bool ecs_os_has_logging(void);

/** Are dynamic library functions available? */
FLECS_API
bool ecs_os_has_dl(void);

/** Are module path functions available? */
FLECS_API
bool ecs_os_has_modules(void);


/** @} */

#endif



/**
 * @defgroup api_types API types
 * @brief Public API types.
 * @{
 */

/**
 * @defgroup core_types Core API Types
 * @brief Types for core API objects.
 * @{
 */

/** Ids are the things that can be added to an entity. 
 * An id can be an entity or pair, and can have optional id flags. */
typedef uint64_t ecs_id_t;

/** An entity identifier.
 * Entity ids consist out of a number unique to the entity in the lower 32 bits, 
 * and a counter used to track entity liveliness in the upper 32 bits. When an 
 * id is recycled, its generation count is increased. This causes recycled ids 
 * to be very large (>4 billion), which is normal. */
typedef ecs_id_t ecs_entity_t;

/** A type is a list of (component) ids.
 * Types are used to communicate the "type" of an entity. In most type systems a 
 * typeof operation returns a single type. In ECS however, an entity can have 
 * multiple components, which is why an ECS type consists of a vector of ids.
 * 
 * The component ids of a type are sorted, which ensures that it doesn't matter
 * in which order components are added to an entity. For example, if adding
 * Position then Velocity would result in type [Position, Velocity], first 
 * adding Velocity then Position would also result in type [Position, Velocity].
 * 
 * Entities are grouped together by type in the ECS storage in tables. The 
 * storage has exactly one table per unique type that is created by the 
 * application that stores all entities and components for that type. This is
 * also referred to as an archetype.
 */
typedef struct {
    ecs_id_t *array;
    int32_t count;
} ecs_type_t;

/** A world is the container for all ECS data and supporting features. 
 * Applications can have multiple worlds, though in most cases will only need
 * one. Worlds are isolated from each other, and can have separate sets of
 * systems, components, modules etc.
 * 
 * If an application has multiple worlds with overlapping components, it is 
 * common (though not strictly required) to use the same component ids across
 * worlds, which can be achieved by declaring a global component id variable.
 * To do this in the C API, see the entities/fwd_component_decl example. The
 * C++ API automatically synchronizes component ids between worlds.
 * 
 * Component id conflicts between worlds can occur when a world has already used
 * an id for something else. There are a few ways to avoid this:
 * 
 * - Ensure to register the same components in each world, in the same order.
 * - Create a dummy world in which all components are preregistered which 
 *   initializes the global id variables.
 * 
 * In some use cases, typically when writing tests, multiple worlds are created 
 * and deleted with different components, registered in different order. To 
 * ensure isolation between tests, the C++ API has a `flecs::reset` function
 * that forces the API to ignore the old component ids. */
typedef struct ecs_world_t ecs_world_t;

/** A table stores entities and components for a specific type. */
typedef struct ecs_table_t ecs_table_t;

/** A term is a single element in a query. */
typedef struct ecs_term_t ecs_term_t;

/** A filter is an iterable data structure that describes a query.
 * Filters are used by the various query implementations in Flecs, like queries,
 * observers and rules, to describe a query. Filters themselves can also be 
 * iterated. */
typedef struct ecs_filter_t ecs_filter_t;

/** A query that caches its results. 
 * Queries are the fastest mechanism for finding and iterating over entities.
 * Queries cache results as a list of matching tables (vs. individual entities).
 * 
 * This has several advantages:
 * - Matching is only performed when new tables are created, which is infrequent
 * - Iterating a query just walks over the cache, no actual searching is needed
 * - Iteration is table-based, which allows for direct iteration of underlying
 *   component arrays, providing good cache locality.
 * 
 * While queries are the fastest mechanism to iterate entiites, they are slower
 * to create than other mechanisms, as a result of having to build the cache 
 * first. For this reason queries are best suited for use cases where a single
 * query can be reused many times (like is the case for systems).
 * 
 * For ad-hoc queries it is recommended to use filters or rules instead, which 
 * are slower to iterate, but much faster to create. Applications should at all
 * times avoid frequent creation/deletion of queries. */
typedef struct ecs_query_t ecs_query_t;

/** A rule is a query with advanced graph traversal features.
 * Rules are fast uncached queries with support for advanced graph features such
 * as the usage of query variables. A simple example of a rule that matches all
 * spaceship entities docked to a planet:
 *   SpaceShip, (DockedTo, $planet), Planet($planet)
 * 
 * Here, the rule traverses the DockedTo relationship, and matches Planet on the
 * target of this relationship. Through the usage of variables rules can match
 * arbitrary patterns against entity graphs. Other features supported 
 * exclusively by rules are:
 * - Component inheritance
 * - Transitivity
 * 
 * Rules have similar iteration performance to filters, but are slower than 
 * queries. Rules and filters will eventually be merged into a single query
 * implementation. Features still lacking for rules are:
 * - Up traversal
 * - AndFrom, OrFrom, NotFrom operators
 */
typedef struct ecs_rule_t ecs_rule_t;

/** An observer is a system that is invoked when an event matches its query.
 * Observers allow applications to respond to specific events, such as adding or
 * removing a component. Observers are created by both specifying a query and
 * a list of event kinds that should be listened for. An example of an observer
 * that triggers when a Position component is added to an entity (in C++):
 * 
 *   world.observer<Position>()
 *     .event(flecs::OnAdd)
 *     .each([](Position& p) {
 *       // called when Position is added to an entity
 *     });
 * 
 * Observer queries can be as complex as filters. Observers only trigger when
 * the source of the event matches the full observer query. For example, an 
 * OnAdd observer for Position, Velocity will only trigger after both components
 * have been added to the entity. */
typedef struct ecs_observer_t ecs_observer_t;

/** An observable produces events that can be listened for by an observer.
 * Currently only the world is observable. In the future, queries will become
 * observable objects as well. */
typedef struct ecs_observable_t ecs_observable_t;

/* Type used for iterating iterable objects. 
 * Iterators are a common interface across iterable objects (world, filters, 
 * rules, queries, systems, observers) to provide applications with information
 * about the currently iterated result, and to store any state required for the 
 * iteration. */
typedef struct ecs_iter_t ecs_iter_t;

/** A ref is a fast way to fetch a component for a specific entity.
 * Refs are a faster alternative to repeatedly calling ecs_get for the same
 * entity/component combination. When comparing the performance of getting a ref
 * to calling ecs_get, a ref is typically 3-5x faster.
 * 
 * Refs achieve this performance by caching internal data structures associated
 * with the entity and component on the ecs_ref_t object that otherwise would 
 * have to be looked up. */
typedef struct ecs_ref_t ecs_ref_t;

/** Type hooks are callbacks associated with component lifecycle events. 
 * Typical examples of lifecycle events are construction, destruction, copying
 * and moving of components. */
typedef struct ecs_type_hooks_t ecs_type_hooks_t;

/** Type information.
 * Contains information about a (component) type, such as its size and 
 * alignment and type hooks. */
typedef struct ecs_type_info_t ecs_type_info_t;

/** Information about an entity, like its table and row. */
typedef struct ecs_record_t ecs_record_t;

/** Information about a (component) id, such as type info and tables with the id */
typedef struct ecs_id_record_t ecs_id_record_t;

/** Information about where in a table a specific (component) id is stored. */
typedef struct ecs_table_record_t ecs_table_record_t;

/** A poly object.
 * A poly (short for polymorph) object is an object that has a variable list of
 * capabilities, determined by a mixin table. This is the current list of types
 * in the flecs API that can be used as an ecs_poly_t:
 * 
 * - ecs_world_t
 * - ecs_stage_t
 * - ecs_query_t
 * - ecs_filter_t
 * - ecs_rule_t
 * - (more to come)
 * 
 * Functions that accept an ecs_poly_t argument can accept objects of these
 * types. If the object does not have the requested mixin the API will throw an
 * assert.
 * 
 * The poly/mixin framework enables partially overlapping features to be
 * implemented once, and enables objects of different types to interact with
 * each other depending on what mixins they have, rather than their type
 * (in some ways it's like a mini-ECS). Additionally, each poly object has a
 * header that enables the API to do sanity checking on the input arguments.
 */
typedef void ecs_poly_t;

/** Type that stores poly mixins */
typedef struct ecs_mixins_t ecs_mixins_t;

/** Header for ecs_poly_t objects. */
typedef struct ecs_header_t {
    int32_t magic; /* Magic number verifying it's a flecs object */
    int32_t type;  /* Magic number indicating which type of flecs object */
    ecs_mixins_t *mixins; /* Table with offsets to (optional) mixins */
} ecs_header_t;

/** @} */

/**
 * @defgroup function_types Function types.
 * @brief Function callback types.
 * @{
 */

/** Function prototype for runnables (systems, observers).
 * The run callback overrides the default behavior for iterating through the
 * results of a runnable object.
 * 
 * The default runnable iterates the iterator, and calls an iter_action (see
 * below) for each returned result.
 * 
 * @param it The iterator to be iterated by the runnable.
 */
typedef void (*ecs_run_action_t)(
    ecs_iter_t *it);

/** Function prototype for iterables.
 * A system may invoke a callback multiple times, typically once for each
 * matched table.
 * 
 * @param it The iterator containing the data for the current match.
 */
typedef void (*ecs_iter_action_t)(
    ecs_iter_t *it);

/** Function prototype for creating an iterator from a poly.
 * Used to create iterators from poly objects with the iterable mixin. When a
 * filter is provided, an array of two iterators must be passed to the function.
 * This allows the mixin implementation to create a chained iterator when
 * necessary, which requires two iterator objects.
 * 
 * @param world The world or stage for which to create the iterator.
 * @param iterable An iterable poly object.
 * @param it The iterator to create (out parameter)
 * @param filter Optional term to filter results.
 */
typedef void (*ecs_iter_init_action_t)(
    const ecs_world_t *world,
    const ecs_poly_t *iterable,
    ecs_iter_t *it,
    ecs_term_t *filter);

/** Function prototype for iterating an iterator.
 * Stored inside initialized iterators. This allows an application to * iterate 
 * an iterator without needing to know what created it.
 * 
 * @param it The iterator to iterate.
 * @return True if iterator has no more results, false if it does.
 */
typedef bool (*ecs_iter_next_action_t)(
    ecs_iter_t *it);  

/** Function prototype for freeing an iterator.
 * Free iterator resources.
 * 
 * @param it The iterator to free.
 */
typedef void (*ecs_iter_fini_action_t)(
    ecs_iter_t *it); 

/** Callback used for comparing components */
typedef int (*ecs_order_by_action_t)(
    ecs_entity_t e1,
    const void *ptr1,
    ecs_entity_t e2,
    const void *ptr2);

/** Callback used for sorting the entire table of components */
typedef void (*ecs_sort_table_action_t)(
    ecs_world_t* world,
    ecs_table_t* table,
    ecs_entity_t* entities,
    void* ptr,
    int32_t size,
    int32_t lo,
    int32_t hi,
    ecs_order_by_action_t order_by);

/** Callback used for grouping tables in a query */
typedef uint64_t (*ecs_group_by_action_t)(
    ecs_world_t *world,
    ecs_table_t *table,
    ecs_id_t group_id,
    void *ctx);

/* Callback invoked when a query creates a new group. */
typedef void* (*ecs_group_create_action_t)(
    ecs_world_t *world,
    uint64_t group_id,
    void *group_by_ctx); /* from ecs_query_desc_t */

/* Callback invoked when a query deletes an existing group. */
typedef void (*ecs_group_delete_action_t)(
    ecs_world_t *world,
    uint64_t group_id,
    void *group_ctx,     /* return value from ecs_group_create_action_t */
    void *group_by_ctx); /* from ecs_query_desc_t */

/** Initialization action for modules */
typedef void (*ecs_module_action_t)(
    ecs_world_t *world);    

/** Action callback on world exit */
typedef void (*ecs_fini_action_t)(
    ecs_world_t *world,
    void *ctx);

/** Function to cleanup context data */
typedef void (*ecs_ctx_free_t)(
    void *ctx);

/** Callback used for sorting values */
typedef int (*ecs_compare_action_t)(
    const void *ptr1,
    const void *ptr2);

/** Callback used for hashing values */
typedef uint64_t (*ecs_hash_value_action_t)(
    const void *ptr); 

/** Constructor/destructor callback */
typedef void (*ecs_xtor_t)(
    void *ptr,
    int32_t count,
    const ecs_type_info_t *type_info);

/** Copy is invoked when a component is copied into another component. */
typedef void (*ecs_copy_t)(
    void *dst_ptr,
    const void *src_ptr,
    int32_t count,
    const ecs_type_info_t *type_info);

/** Move is invoked when a component is moved to another component. */
typedef void (*ecs_move_t)(
    void *dst_ptr,
    void *src_ptr,
    int32_t count,
    const ecs_type_info_t *type_info);

/* Destructor function for poly objects */
typedef void (*ecs_poly_dtor_t)(
    ecs_poly_t *poly);

/** @} */

/**
 * @defgroup mixins Poly mixin types.
 * @brief Mixin types for poly mechanism.
 * @{
 */

/** Iterable mixin.
 * Allows its container to be iterated. */
typedef struct ecs_iterable_t {
    ecs_iter_init_action_t init; /**< Callback that creates iterator. */
} ecs_iterable_t;

/** @} */

/**
 * @defgroup query_types Query descriptor types.
 * @brief Types used to describe queries.
 * @{
 */

/** Specify read/write access for term */
typedef enum ecs_inout_kind_t {
    EcsInOutDefault,  /**< InOut for regular terms, In for shared terms */
    EcsInOutNone,     /**< Term is neither read nor written */
    EcsInOut,         /**< Term is both read and written */
    EcsIn,            /**< Term is only read */
    EcsOut,           /**< Term is only written */
} ecs_inout_kind_t;

/** Specify operator for term */
typedef enum ecs_oper_kind_t {
    EcsAnd,           /**< The term must match */
    EcsOr,            /**< One of the terms in an or chain must match */
    EcsNot,           /**< The term must not match */
    EcsOptional,      /**< The term may match */
    EcsAndFrom,       /**< Term must match all components from term id */
    EcsOrFrom,        /**< Term must match at least one component from term id */
    EcsNotFrom,       /**< Term must match none of the components from term id */
} ecs_oper_kind_t;

/* Term id flags  */
#define EcsSelf                       (1u << 1)  /**< Match on self */
#define EcsUp                         (1u << 2)  /**< Match by traversing upwards */
#define EcsDown                       (1u << 3)  /**< Match by traversing downwards (derived, cannot be set) */
#define EcsTraverseAll                (1u << 4)  /**< Match all entities encountered through traversal */
#define EcsCascade                    (1u << 5)  /**< Sort results breadth first */
#define EcsDesc                       (1u << 6)  /**< Iterate groups in descending order  */
#define EcsParent                     (1u << 7)  /**< Short for up(ChildOf) */
#define EcsIsVariable                 (1u << 8)  /**< Term id is a variable */
#define EcsIsEntity                   (1u << 9)  /**< Term id is an entity */
#define EcsIsName                     (1u << 10) /**< Term id is a name (don't attempt to lookup as entity) */
#define EcsFilter                     (1u << 11) /**< Prevent observer from triggering on term */
#define EcsTraverseFlags              (EcsUp|EcsDown|EcsTraverseAll|EcsSelf|EcsCascade|EcsDesc|EcsParent)

/* Term flags discovered & set during filter creation. Mostly used internally to
 * store information relevant to queries. */
#define EcsTermMatchAny               (1u << 0)
#define EcsTermMatchAnySrc            (1u << 1)
#define EcsTermSrcFirstEq             (1u << 2)
#define EcsTermSrcSecondEq            (1u << 3)
#define EcsTermTransitive             (1u << 4)
#define EcsTermReflexive              (1u << 5)
#define EcsTermIdInherited            (1u << 6)
#define EcsTermIsTrivial              (1u << 7)
#define EcsTermNoData                 (1u << 8)

/* Term flags used for term iteration */
#define EcsTermMatchDisabled          (1u << 7)
#define EcsTermMatchPrefab            (1u << 8)

/** Type that describes a single identifier in a term */
typedef struct ecs_term_id_t {
    ecs_entity_t id;            /**< Entity id. If left to 0 and flags does not 
                                 * specify whether id is an entity or a variable
                                 * the id will be initialized to EcsThis. 
                                 * To explicitly set the id to 0, leave the id
                                 * member to 0 and set EcsIsEntity in flags. */

    const char *name;           /**< Name. This can be either the variable name
                                 * (when the EcsIsVariable flag is set) or an
                                 * entity name. When ecs_term_t::move is true,
                                 * the API assumes ownership over the string and
                                 * will free it when the term is destroyed. */

    ecs_entity_t trav;          /**< Relationship to traverse when looking for the
                                 * component. The relationship must have
                                 * the Traversable property. Default is IsA. */

    ecs_flags32_t flags;        /**< Term flags */
} ecs_term_id_t;

/** Type that describes a term (single element in a query) */
struct ecs_term_t {
    ecs_id_t id;                /**< Component id to be matched by term. Can be
                                 * set directly, or will be populated from the
                                 * first/second members, which provide more
                                 * flexibility. */

    ecs_term_id_t src;          /**< Source of term */
    ecs_term_id_t first;        /**< Component or first element of pair */
    ecs_term_id_t second;       /**< Second element of pair */
    
    ecs_inout_kind_t inout;     /**< Access to contents matched by term */
    ecs_oper_kind_t oper;       /**< Operator of term */

    ecs_id_t id_flags;          /**< Id flags of term id */
    char *name;                 /**< Name of term */

    int32_t field_index;        /**< Index of field for term in iterator */
    ecs_id_record_t *idr;       /**< Cached pointer to internal index */

    ecs_flags16_t flags;        /**< Flags that help eval, set by ecs_filter_init */

    bool move;                  /**< Used by internals */
};

/** Use $this variable to initialize user-allocated filter object */
FLECS_API extern ecs_filter_t ECS_FILTER_INIT;

/** Filters alllow for ad-hoc quick filtering of entity tables. */
struct ecs_filter_t {
    ecs_header_t hdr;
    
    int8_t term_count;        /**< Number of elements in terms array */
    int8_t field_count;       /**< Number of fields in iterator for filter */
    ecs_flags32_t flags;      /**< Filter flags */
    ecs_flags64_t data_fields; /**< Bitset with fields that have data */
    
    ecs_term_t *terms;         /**< Array containing terms for filter */
    char *variable_names[1];   /**< Placeholder variable names array */
    int32_t *sizes;            /**< Field size (same for each result) */

    /* Mixins */
    ecs_entity_t entity;       /**< Entity associated with filter (optional) */
    ecs_iterable_t iterable;   /**< Iterable mixin */
    ecs_poly_dtor_t dtor;      /**< Dtor mixin */
    ecs_world_t *world;        /**< World mixin */
};

/* An observer reacts to events matching a filter */
struct ecs_observer_t {
    ecs_header_t hdr;
    
    ecs_filter_t filter;        /**< Query for observer */

    /* Observer events */
    ecs_entity_t events[FLECS_EVENT_DESC_MAX];
    int32_t event_count;   
    
    ecs_iter_action_t callback; /**< See ecs_observer_desc_t::callback */
    ecs_run_action_t run;       /**< See ecs_observer_desc_t::run */

    void *ctx;                  /**< Callback context */
    void *binding_ctx;          /**< Binding context (for language bindings) */

    ecs_ctx_free_t ctx_free;    /**< Callback to free ctx */
    ecs_ctx_free_t binding_ctx_free; /**< Callback to free binding_ctx */

    ecs_observable_t *observable; /**< Observable for observer */

    int32_t *last_event_id;     /**< Last handled event id */
    int32_t last_event_id_storage;

    ecs_id_t register_id;       /**< Id observer is registered with (single term observers only) */
    int32_t term_index;         /**< Index of the term in parent observer (single term observers only) */

    bool is_monitor;            /**< If true, the observer only triggers when the
                                 * filter did not match with the entity before
                                 * the event happened. */

    bool is_multi;              /**< If true, the observer triggers on more than one term */

    /* Mixins */
    ecs_poly_dtor_t dtor;
};

/** @} */

/** Type that contains component lifecycle callbacks. 
 * 
 * \ingroup components
 */
struct ecs_type_hooks_t {
    ecs_xtor_t ctor;            /**< ctor */
    ecs_xtor_t dtor;            /**< dtor */
    ecs_copy_t copy;            /**< copy assignment */
    ecs_move_t move;            /**< move assignment */

    /** Ctor + copy */
    ecs_copy_t copy_ctor;

    /** Ctor + move */
    ecs_move_t move_ctor;

    /** Ctor + move + dtor (or move_ctor + dtor).
     * This combination is typically used when a component is moved from one
     * location to a new location, like when it is moved to a new table. If
     * not set explicitly it will be derived from other callbacks. */
    ecs_move_t ctor_move_dtor;

    /** Move + dtor.
     * This combination is typically used when a component is moved from one
     * location to an existing location, like what happens during a remove. If
     * not set explicitly it will be derived from other callbacks. */
    ecs_move_t move_dtor;

    /** Callback that is invoked when an instance of a component is added. This
     * callback is invoked before triggers are invoked. */
    ecs_iter_action_t on_add;

    /** Callback that is invoked when an instance of the component is set. This
     * callback is invoked before triggers are invoked, and enable the component
     * to respond to changes on itself before others can. */
    ecs_iter_action_t on_set;

    /** Callback that is invoked when an instance of the component is removed. 
     * This callback is invoked after the triggers are invoked, and before the
     * destructor is invoked. */
    ecs_iter_action_t on_remove;

    void *ctx;                       /**< User defined context */
    void *binding_ctx;               /**< Language binding context */

    ecs_ctx_free_t ctx_free;         /**< Callback to free ctx */
    ecs_ctx_free_t binding_ctx_free; /**< Callback to free binding_ctx */
};

/** Type that contains component information (passed to ctors/dtors/...) 
 * 
 * \ingroup components
 */
struct ecs_type_info_t {
    ecs_size_t size;         /**< Size of type */
    ecs_size_t alignment;    /**< Alignment of type */
    ecs_type_hooks_t hooks;  /**< Type hooks */
    ecs_entity_t component;  /**< Handle to component (do not set) */
    const char *name;        /**< Type name. */
};

/**
 * @file api_types.h
 * @brief Supporting types for the public API.
 *
 * This file contains types that are typically not used by an application but 
 * support the public API, and therefore must be exposed. This header should not
 * be included by itself.
 */

#ifndef FLECS_API_TYPES_H
#define FLECS_API_TYPES_H



////////////////////////////////////////////////////////////////////////////////
//// Opaque types
////////////////////////////////////////////////////////////////////////////////

/** A stage enables modification while iterating and from multiple threads */
typedef struct ecs_stage_t ecs_stage_t;

/** Table data */
typedef struct ecs_data_t ecs_data_t;

/* Switch list */
typedef struct ecs_switch_t ecs_switch_t;

/* Cached query table data */
typedef struct ecs_query_table_match_t ecs_query_table_match_t;

////////////////////////////////////////////////////////////////////////////////
//// Non-opaque types
////////////////////////////////////////////////////////////////////////////////

/** Mixin for emitting events to triggers/observers */
/** All observers for a specific event */
typedef struct ecs_event_record_t {
    struct ecs_event_id_record_t *any;
    struct ecs_event_id_record_t *wildcard;
    struct ecs_event_id_record_t *wildcard_pair;
    ecs_map_t event_ids; /* map<id, ecs_event_id_record_t> */
    ecs_entity_t event;
} ecs_event_record_t;

struct ecs_observable_t {
    ecs_event_record_t on_add;
    ecs_event_record_t on_remove;
    ecs_event_record_t on_set;
    ecs_event_record_t un_set;
    ecs_event_record_t on_wildcard;
    ecs_sparse_t events;  /* sparse<event, ecs_event_record_t> */
};

/** Record for entity index */
struct ecs_record_t {
    ecs_id_record_t *idr; /* Id record to (*, entity) for target entities */
    ecs_table_t *table;   /* Identifies a type (and table) in world */
    uint32_t row;         /* Table row of the entity */
    int32_t dense;        /* Index in dense array */    
};

/** Range in table */
typedef struct ecs_table_range_t {
    ecs_table_t *table;
    int32_t offset;       /* Leave both members to 0 to cover entire table */
    int32_t count;       
} ecs_table_range_t;

/** Value of query variable */
typedef struct ecs_var_t {
    ecs_table_range_t range; /* Set when variable stores a range of entities */
    ecs_entity_t entity;     /* Set when variable stores single entity */

    /* Most entities can be stored as a range by setting range.count to 1, 
     * however in order to also be able to store empty entities in variables, 
     * a separate entity member is needed. Both range and entity may be set at
     * the same time, as long as they are consistent. */
} ecs_var_t;

/** Cached reference. */
struct ecs_ref_t {
    ecs_entity_t entity;    /* Entity */
    ecs_entity_t id;        /* Component id */
    struct ecs_table_record_t *tr; /* Table record for component */
    ecs_record_t *record;   /* Entity index record */
};

/* Cursor to stack allocator. Type is public to allow for white box testing. */
struct ecs_stack_page_t;

typedef struct ecs_stack_cursor_t {
    struct ecs_stack_cursor_t *prev;
    struct ecs_stack_page_t *page;
    int16_t sp;
    bool is_free;
#ifdef FLECS_DEBUG
    struct ecs_stack_t *owner;
#endif
} ecs_stack_cursor_t;

/* Page-iterator specific data */
typedef struct ecs_page_iter_t {
    int32_t offset;
    int32_t limit;
    int32_t remaining;
} ecs_page_iter_t;

/* Worker-iterator specific data */
typedef struct ecs_worker_iter_t {
    int32_t index;
    int32_t count;
} ecs_worker_iter_t;

/* Convenience struct to iterate table array for id */
typedef struct ecs_table_cache_iter_t {
    struct ecs_table_cache_hdr_t *cur, *next;
    struct ecs_table_cache_hdr_t *next_list;
} ecs_table_cache_iter_t;

/** Term-iterator specific data */
typedef struct ecs_term_iter_t {
    ecs_term_t term;
    ecs_id_record_t *self_index;
    ecs_id_record_t *set_index;

    ecs_id_record_t *cur;
    ecs_table_cache_iter_t it;
    int32_t index;
    int32_t observed_table_count;
    
    ecs_table_t *table;
    int32_t cur_match;
    int32_t match_count;
    int32_t last_column;

    bool empty_tables;

    /* Storage */
    ecs_id_t id;
    int32_t column;
    ecs_entity_t subject;
    ecs_size_t size;
    void *ptr;
} ecs_term_iter_t;

typedef enum ecs_iter_kind_t {
    EcsIterEvalCondition,
    EcsIterEvalTables,
    EcsIterEvalChain,
    EcsIterEvalNone
} ecs_iter_kind_t;

/** Filter-iterator specific data */
typedef struct ecs_filter_iter_t {
    const ecs_filter_t *filter;
    ecs_iter_kind_t kind; 
    ecs_term_iter_t term_iter;
    int32_t matches_left;
    int32_t pivot_term;
} ecs_filter_iter_t;

/** Query-iterator specific data */
typedef struct ecs_query_iter_t {
    ecs_query_t *query;
    ecs_query_table_match_t *node, *prev, *last;
    int32_t sparse_smallest;
    int32_t sparse_first;
    int32_t bitset_first;
    int32_t skip_count;
} ecs_query_iter_t;

/** Snapshot-iterator specific data */
typedef struct ecs_snapshot_iter_t {
    ecs_filter_t filter;
    ecs_vec_t tables; /* ecs_table_leaf_t */
    int32_t index;
} ecs_snapshot_iter_t;

typedef struct ecs_rule_op_profile_t {
    int32_t count[2]; /* 0 = enter, 1 = redo */
} ecs_rule_op_profile_t;

/** Rule-iterator specific data */
typedef struct ecs_rule_iter_t {
    const ecs_rule_t *rule;
    struct ecs_var_t *vars;              /* Variable storage */
    const struct ecs_rule_var_t *rule_vars;
    const struct ecs_rule_op_t *ops;
    struct ecs_rule_op_ctx_t *op_ctx;    /* Operation-specific state */
    uint64_t *written;
    ecs_flags32_t source_set;

#ifdef FLECS_DEBUG
    ecs_rule_op_profile_t *profile;
#endif

    int16_t op;
    int16_t sp;
} ecs_rule_iter_t;

/* Bits for tracking whether a cache was used/whether the array was allocated.
 * Used by flecs_iter_init, flecs_iter_validate and ecs_iter_fini. 
 * Constants are named to enable easy macro substitution. */
#define flecs_iter_cache_ids           (1u << 0u)
#define flecs_iter_cache_columns       (1u << 1u)
#define flecs_iter_cache_sources       (1u << 2u)
#define flecs_iter_cache_ptrs          (1u << 3u)
#define flecs_iter_cache_match_indices (1u << 4u)
#define flecs_iter_cache_variables     (1u << 5u)
#define flecs_iter_cache_all           (255)

/* Inline iterator arrays to prevent allocations for small array sizes */
typedef struct ecs_iter_cache_t {
    ecs_stack_cursor_t *stack_cursor; /* Stack cursor to restore to */
    ecs_flags8_t used;       /* For which fields is the cache used */
    ecs_flags8_t allocated;  /* Which fields are allocated */
} ecs_iter_cache_t;

/* Private iterator data. Used by iterator implementations to keep track of
 * progress & to provide builtin storage. */
typedef struct ecs_iter_private_t {
    union {
        ecs_term_iter_t term;
        ecs_filter_iter_t filter;
        ecs_query_iter_t query;
        ecs_rule_iter_t rule;
        ecs_snapshot_iter_t snapshot;
        ecs_page_iter_t page;
        ecs_worker_iter_t worker;
    } iter;                       /* Iterator specific data */

    void *entity_iter;            /* Filter applied after matching a table */
    ecs_iter_cache_t cache;       /* Inline arrays to reduce allocations */
} ecs_iter_private_t;

/** Iterator */
struct ecs_iter_t {
    /* World */
    ecs_world_t *world;           /* The world */
    ecs_world_t *real_world;      /* Actual world. This differs from world when in readonly mode */

    /* Matched data */
    ecs_entity_t *entities;       /* Entity identifiers */
    void **ptrs;                  /* Pointers to components. Array if from this, pointer if not. */
    ecs_size_t *sizes;            /* Component sizes */
    ecs_table_t *table;           /* Current table */
    ecs_table_t *other_table;     /* Prev or next table when adding/removing */
    ecs_id_t *ids;                /* (Component) ids */
    ecs_var_t *variables;         /* Values of variables (if any) */
    int32_t *columns;             /* Query term to table column mapping */
    ecs_entity_t *sources;        /* Entity on which the id was matched (0 if same as entities) */
    int32_t *match_indices;       /* Indices of current match for term. Allows an iterator to iterate
                                   * all permutations of wildcards in query. */
    ecs_ref_t *references;        /* Cached refs to components (if iterating a cache) */
    ecs_flags64_t constrained_vars; /* Bitset that marks constrained variables */
    uint64_t group_id;            /* Group id for table, if group_by is used */
    int32_t field_count;          /* Number of fields in iterator */

    /* Input information */
    ecs_entity_t system;          /* The system (if applicable) */
    ecs_entity_t event;           /* The event (if applicable) */
    ecs_id_t event_id;            /* The (component) id for the event */

    /* Query information */
    ecs_term_t *terms;            /* Terms of query being evaluated */
    int32_t table_count;          /* Active table count for query */
    int32_t term_index;           /* Index of term that emitted an event.
                                   * This field will be set to the 'index' field
                                   * of an observer term. */
    int32_t variable_count;       /* Number of variables for query */
    char **variable_names;        /* Names of variables (if any) */

    /* Context */
    void *param;                  /* Param passed to ecs_run */
    void *ctx;                    /* System context */
    void *binding_ctx;            /* Binding context */

    /* Time */
    ecs_ftime_t delta_time;       /* Time elapsed since last frame */
    ecs_ftime_t delta_system_time;/* Time elapsed since last system invocation */

    /* Iterator counters */
    int32_t frame_offset;         /* Offset relative to start of iteration */
    int32_t offset;               /* Offset relative to current table */
    int32_t count;                /* Number of entities to iterate */
    int32_t instance_count;       /* Number of entities to iterate before next table */

    /* Iterator flags */
    ecs_flags32_t flags;

    ecs_entity_t interrupted_by;  /* When set, system execution is interrupted */

    ecs_iter_private_t priv;      /* Private data */

    /* Chained iterators */
    ecs_iter_next_action_t next;  /* Function to progress iterator */
    ecs_iter_action_t callback;   /* Callback of system or observer */
    ecs_iter_action_t set_var;    /* Invoked after setting variable (optionally set) */
    ecs_iter_fini_action_t fini;  /* Function to cleanup iterator resources */
    ecs_iter_t *chain_it;         /* Optional, allows for creating iterator chains */
};


#endif


/**
 * @file api_support.h
 * @brief Support functions and constants.
 *
 * Supporting types and functions that need to be exposed either in support of 
 * the public API or for unit tests, but that may change between minor / patch 
 * releases. 
 */

#ifndef FLECS_API_SUPPORT_H
#define FLECS_API_SUPPORT_H



/** This is the largest possible component id. Components for the most part
 * occupy the same id range as entities, however they are not allowed to overlap
 * with (8) bits reserved for id flags. */
#define ECS_MAX_COMPONENT_ID (~((uint32_t)(ECS_ID_FLAGS_MASK >> 32)))

/** The maximum number of nested function calls before the core will throw a
 * cycle detected error */
#define ECS_MAX_RECURSION (512)

/** Maximum length of a parser token (used by parser-related addons) */
#define ECS_MAX_TOKEN_SIZE (256)

////////////////////////////////////////////////////////////////////////////////
//// Global type handles
////////////////////////////////////////////////////////////////////////////////

/** This allows passing 0 as type to functions that accept ids */
#define FLECS_ID0ID_ 0

FLECS_API
char* ecs_module_path_from_c(
    const char *c_name);

bool ecs_identifier_is_0(
    const char *id);

/* Constructor that zeromem's a component value */
FLECS_API
void ecs_default_ctor(
    void *ptr, 
    int32_t count, 
    const ecs_type_info_t *ctx);

/* Create allocated string from format */
FLECS_DBG_API
char* ecs_vasprintf(
    const char *fmt,
    va_list args);

/* Create allocated string from format */
FLECS_API
char* ecs_asprintf(
    const char *fmt,
    ...);

/* Convert identifier to snake case */
FLECS_API
char* flecs_to_snake_case(
    const char *str);

FLECS_DBG_API
int32_t flecs_table_observed_count(
    const ecs_table_t *table);

FLECS_DBG_API
void flecs_dump_backtrace(
    void *stream);

/** Calculate offset from address */
#define ECS_OFFSET(o, offset) (void*)(((uintptr_t)(o)) + ((uintptr_t)(offset)))
#define ECS_OFFSET_T(o, T) ECS_OFFSET(o, ECS_SIZEOF(T))

#define ECS_ELEM(ptr, size, index) ECS_OFFSET(ptr, (size) * (index))
#define ECS_ELEM_T(o, T, index) ECS_ELEM(o, ECS_SIZEOF(T), index)

/** Enable/disable bitsets */
#define ECS_BIT_SET(flags, bit) (flags) |= (bit)
#define ECS_BIT_CLEAR(flags, bit) (flags) &= ~(bit) 
#define ECS_BIT_COND(flags, bit, cond) ((cond) \
    ? (ECS_BIT_SET(flags, bit)) \
    : (ECS_BIT_CLEAR(flags, bit)))
#define ECS_BIT_IS_SET(flags, bit) ((flags) & (bit))


#endif

/**
 * @file hashmap.h
 * @brief Hashmap data structure.
 */

#ifndef FLECS_HASHMAP_H
#define FLECS_HASHMAP_H



typedef struct {
    ecs_vec_t keys;
    ecs_vec_t values;
} ecs_hm_bucket_t;

typedef struct {
    ecs_hash_value_action_t hash;
    ecs_compare_action_t compare;
    ecs_size_t key_size;
    ecs_size_t value_size;
    ecs_block_allocator_t *hashmap_allocator;
    ecs_block_allocator_t bucket_allocator;
    ecs_map_t impl;
} ecs_hashmap_t;

typedef struct {
    ecs_map_iter_t it;
    ecs_hm_bucket_t *bucket;
    int32_t index;
} flecs_hashmap_iter_t;

typedef struct {
    void *key;
    void *value;
    uint64_t hash;
} flecs_hashmap_result_t;

FLECS_DBG_API
void flecs_hashmap_init_(
    ecs_hashmap_t *hm,
    ecs_size_t key_size,
    ecs_size_t value_size,
    ecs_hash_value_action_t hash,
    ecs_compare_action_t compare,
    ecs_allocator_t *allocator);

#define flecs_hashmap_init(hm, K, V, hash, compare, allocator)\
    flecs_hashmap_init_(hm, ECS_SIZEOF(K), ECS_SIZEOF(V), hash, compare, allocator)

FLECS_DBG_API
void flecs_hashmap_fini(
    ecs_hashmap_t *map);

FLECS_DBG_API
void* flecs_hashmap_get_(
    const ecs_hashmap_t *map,
    ecs_size_t key_size,
    const void *key,
    ecs_size_t value_size);

#define flecs_hashmap_get(map, key, V)\
    (V*)flecs_hashmap_get_(map, ECS_SIZEOF(*key), key, ECS_SIZEOF(V))

FLECS_DBG_API
flecs_hashmap_result_t flecs_hashmap_ensure_(
    ecs_hashmap_t *map,
    ecs_size_t key_size,
    const void *key,
    ecs_size_t value_size);

#define flecs_hashmap_ensure(map, key, V)\
    flecs_hashmap_ensure_(map, ECS_SIZEOF(*key), key, ECS_SIZEOF(V))

FLECS_DBG_API
void flecs_hashmap_set_(
    ecs_hashmap_t *map,
    ecs_size_t key_size,
    void *key,
    ecs_size_t value_size,
    const void *value);

#define flecs_hashmap_set(map, key, value)\
    flecs_hashmap_set_(map, ECS_SIZEOF(*key), key, ECS_SIZEOF(*value), value)

FLECS_DBG_API
void flecs_hashmap_remove_(
    ecs_hashmap_t *map,
    ecs_size_t key_size,
    const void *key,
    ecs_size_t value_size);

#define flecs_hashmap_remove(map, key, V)\
    flecs_hashmap_remove_(map, ECS_SIZEOF(*key), key, ECS_SIZEOF(V))

FLECS_DBG_API
void flecs_hashmap_remove_w_hash_(
    ecs_hashmap_t *map,
    ecs_size_t key_size,
    const void *key,
    ecs_size_t value_size,
    uint64_t hash);

#define flecs_hashmap_remove_w_hash(map, key, V, hash)\
    flecs_hashmap_remove_w_hash_(map, ECS_SIZEOF(*key), key, ECS_SIZEOF(V), hash)

FLECS_DBG_API
ecs_hm_bucket_t* flecs_hashmap_get_bucket(
    const ecs_hashmap_t *map,
    uint64_t hash);

FLECS_DBG_API
void flecs_hm_bucket_remove(
    ecs_hashmap_t *map,
    ecs_hm_bucket_t *bucket,
    uint64_t hash,
    int32_t index);

FLECS_DBG_API
void flecs_hashmap_copy(
    ecs_hashmap_t *dst,
    const ecs_hashmap_t *src);

FLECS_DBG_API
flecs_hashmap_iter_t flecs_hashmap_iter(
    ecs_hashmap_t *map);

FLECS_DBG_API
void* flecs_hashmap_next_(
    flecs_hashmap_iter_t *it,
    ecs_size_t key_size,
    void *key_out,
    ecs_size_t value_size);

#define flecs_hashmap_next(map, V)\
    (V*)flecs_hashmap_next_(map, 0, NULL, ECS_SIZEOF(V))

#define flecs_hashmap_next_w_key(map, K, key, V)\
    (V*)flecs_hashmap_next_(map, ECS_SIZEOF(K), key, ECS_SIZEOF(V))


#endif


/** Used with ecs_entity_init 
 * 
 * \ingroup entities
 */ 
typedef struct ecs_entity_desc_t {
    int32_t _canary;

    ecs_entity_t id;      /**< Set to modify existing entity (optional) */

    const char *name;     /**< Name of the entity. If no entity is provided, an
                           * entity with this name will be looked up first. When
                           * an entity is provided, the name will be verified
                           * with the existing entity. */

    const char *sep;      /**< Optional custom separator for hierarchical names.
                           * Leave to NULL for default ('.') separator. Set to
                           * an empty string to prevent tokenization of name. */

    const char *root_sep; /**< Optional, used for identifiers relative to root */

    const char *symbol;   /**< Optional entity symbol. A symbol is an unscoped
                           * identifier that can be used to lookup an entity. The
                           * primary use case for this is to associate the entity
                           * with a language identifier, such as a type or
                           * function name, where these identifiers differ from
                           * the name they are registered with in flecs. For
                           * example, C type "EcsPosition" might be registered
                           * as "flecs.components.transform.Position", with the
                           * symbol set to "EcsPosition". */

    bool use_low_id;      /**< When set to true, a low id (typically reserved for
                           * components) will be used to create the entity, if
                           * no id is specified. */

    /** Array of ids to add to the new or existing entity. */
    ecs_id_t add[FLECS_ID_DESC_MAX];

    /** String expression with components to add */
    const char *add_expr;
} ecs_entity_desc_t;

/** Used with ecs_bulk_init 
 * 
 * \ingroup entities
 */
typedef struct ecs_bulk_desc_t { 
    int32_t _canary;

    ecs_entity_t *entities; /**< Entities to bulk insert. Entity ids provided by
                             * the application must be empty (cannot
                             * have components). If no entity ids are provided, the
                             * operation will create 'count' new entities. */

    int32_t count;     /**< Number of entities to create/populate */

    ecs_id_t ids[FLECS_ID_DESC_MAX]; /**< Ids to create the entities with */

    void **data;       /**< Array with component data to insert. Each element in 
                        * the array must correspond with an element in the ids
                        * array. If an element in the ids array is a tag, the
                        * data array must contain a NULL. An element may be
                        * set to NULL for a component, in which case the
                        * component will not be set by the operation. */

    ecs_table_t *table; /**< Table to insert the entities into. Should not be set
                         * at the same time as ids. When 'table' is set at the
                         * same time as 'data', the elements in the data array
                         * must correspond with the ids in the table's type. */

} ecs_bulk_desc_t;

/** Used with ecs_component_init. 
 * 
 * \ingroup components
 */
typedef struct ecs_component_desc_t {
    int32_t _canary;
    
    /** Existing entity to associate with observer (optional) */
    ecs_entity_t entity;

    /** Parameters for type (size, hooks, ...) */
    ecs_type_info_t type;
} ecs_component_desc_t;

/** Used with ecs_filter_init. 
 * 
 * \ingroup filters
 */
typedef struct ecs_filter_desc_t {
    int32_t _canary;

    /** Terms of the filter. If a filter has more terms than 
     * FLECS_TERM_DESC_MAX use terms_buffer */
    ecs_term_t terms[FLECS_TERM_DESC_MAX];

    /** For filters with lots of terms an outside array can be provided. */
    ecs_term_t *terms_buffer;

    /** Number of terms in array provided in terms_buffer. */
    int32_t terms_buffer_count;

    /** External storage to prevent allocation of the filter object */
    ecs_filter_t *storage;

    /** When true, terms returned by an iterator may either contain 1 or N 
     * elements, where terms with N elements are owned, and terms with 1 element 
     * are shared, for example from a parent or base entity. When false, the 
     * iterator will at most return 1 element when the result contains both 
     * owned and shared terms. */ 
    bool instanced;

    /** Flags for advanced usage */
    ecs_flags32_t flags;

    /** Filter expression. Should not be set at the same time as terms array */
    const char *expr;

    /** Entity associated with query (optional) */
    ecs_entity_t entity;
} ecs_filter_desc_t;

/** Used with ecs_query_init. 
 * 
 * \ingroup queries
 */
typedef struct ecs_query_desc_t {
    int32_t _canary;

    /** Filter for the query */
    ecs_filter_desc_t filter;

    /** Component to be used by order_by */
    ecs_entity_t order_by_component;

    /** Callback used for ordering query results. If order_by_id is 0, the 
     * pointer provided to the callback will be NULL. If the callback is not
     * set, results will not be ordered. */
    ecs_order_by_action_t order_by;

    /** Callback used for ordering query results. Same as order_by,
     * but more efficient. */
    ecs_sort_table_action_t sort_table;

    /** Id to be used by group_by. This id is passed to the group_by function and
     * can be used identify the part of an entity type that should be used for
     * grouping. */
    ecs_id_t group_by_id;

    /** Callback used for grouping results. If the callback is not set, results
     * will not be grouped. When set, this callback will be used to calculate a
     * "rank" for each entity (table) based on its components. This rank is then
     * used to sort entities (tables), so that entities (tables) of the same
     * rank are "grouped" together when iterated. */
    ecs_group_by_action_t group_by;

    /** Callback that is invoked when a new group is created. The return value of
     * the callback is stored as context for a group. */
    ecs_group_create_action_t on_group_create;

    /** Callback that is invoked when an existing group is deleted. The return 
     * value of the on_group_create callback is passed as context parameter. */
    ecs_group_delete_action_t on_group_delete;

    /** Context to pass to group_by */
    void *group_by_ctx;

    /** Function to free group_by_ctx */
    ecs_ctx_free_t group_by_ctx_free;

    /** If set, the query will be created as a subquery. A subquery matches at
     * most a subset of its parent query. Subqueries do not directly receive
     * (table) notifications from the world. Instead parent queries forward
     * results to subqueries. This can improve matching performance, as fewer
     * queries need to be matched with new tables.
     * Subqueries can be nested. */
    ecs_query_t *parent;

    /** User context to pass to callback */
    void *ctx;

    /** Context to be used for language bindings */
    void *binding_ctx;
    
    /** Callback to free ctx */
    ecs_ctx_free_t ctx_free;

    /** Callback to free binding_ctx */     
    ecs_ctx_free_t binding_ctx_free;
} ecs_query_desc_t;

/** Used with ecs_observer_init. 
 * 
 * \ingroup observers
 */
typedef struct ecs_observer_desc_t {
    int32_t _canary;

    /** Existing entity to associate with observer (optional) */
    ecs_entity_t entity;

    /** Filter for observer */
    ecs_filter_desc_t filter;

    /** Events to observe (OnAdd, OnRemove, OnSet, UnSet) */
    ecs_entity_t events[FLECS_EVENT_DESC_MAX];

    /** When observer is created, generate events from existing data. For example,
     * EcsOnAdd Position would match all existing instances of Position.
     * This is only supported for events that are iterable (see EcsIterable) */
    bool yield_existing;

    /** Callback to invoke on an event, invoked when the observer matches. */
    ecs_iter_action_t callback;

    /** Callback invoked on an event. When left to NULL the default runner
     * is used which matches the event with the observer's filter, and calls
     * 'callback' when it matches. 
     * A reason to override the run function is to improve performance, if there
     * are more efficient way to test whether an event matches the observer than
     * the general purpose query matcher. */
    ecs_run_action_t run;

    /** User context to pass to callback */
    void *ctx;

    /** Context to be used for language bindings */
    void *binding_ctx;
    
    /** Callback to free ctx */
    ecs_ctx_free_t ctx_free;

    /** Callback to free binding_ctx */     
    ecs_ctx_free_t binding_ctx_free;

    /** Observable with which to register the observer */
    ecs_poly_t *observable;

    /** Optional shared last event id for multiple observers. Ensures only one
     * of the observers with the shared id gets triggered for an event */
    int32_t *last_event_id;

    /** Used for internal purposes */
    int32_t term_index;
} ecs_observer_desc_t;

/** Used with ecs_emit. 
 * 
 * \ingroup observers
 */
typedef struct ecs_event_desc_t {
    /** The event id. Only observers for the specified event will be notified */
    ecs_entity_t event;

    /** Component ids. Only observers with a matching component id will be
     * notified. Observers are guaranteed to get notified once, even if they
     * match more than one id. */
    const ecs_type_t *ids;

    /** The table for which to notify. */
    ecs_table_t *table;

    /** Optional 2nd table to notify. This can be used to communicate the
     * previous or next table, in case an entity is moved between tables. */
    ecs_table_t *other_table;

    /** Limit notified entities to ones starting from offset (row) in table */
    int32_t offset;

    /** Limit number of notified entities to count. offset+count must be less
     * than the total number of entities in the table. If left to 0, it will be
     * automatically determined by doing ecs_table_count(table) - offset. */
    int32_t count;

    /** Single-entity alternative to setting table / offset / count */
    ecs_entity_t entity;

    /** Optional context.
     * The type of the param must be the event, where the event is a component.
     * When an event is enqueued, the value of param is coped to a temporary
     * storage of the event type. */
    void *param;

    /* Same as param, but with the guarantee that the value won't be modified. 
     * When an event with a const parameter is enqueued, the value of the param
     * is copied to a temporary storage of the event type. */
    const void *const_param;

    /** Observable (usually the world) */
    ecs_poly_t *observable;

    /** Event flags */
    ecs_flags32_t flags;
} ecs_event_desc_t;


/**
 * @defgroup misc_types Miscellaneous types
 * @brief Types used to create entities, observers, queries and more.
 * @{
 */

/* Utility to hold a value of a dynamic type */
typedef struct ecs_value_t {
    ecs_entity_t type;
    void *ptr;
} ecs_value_t;

/** Type that contains information about the world. */
typedef struct ecs_world_info_t {
    ecs_entity_t last_component_id;   /**< Last issued component entity id */
    ecs_entity_t min_id;              /**< First allowed entity id */
    ecs_entity_t max_id;              /**< Last allowed entity id */

    ecs_ftime_t delta_time_raw;       /**< Raw delta time (no time scaling) */
    ecs_ftime_t delta_time;           /**< Time passed to or computed by ecs_progress */
    ecs_ftime_t time_scale;           /**< Time scale applied to delta_time */
    ecs_ftime_t target_fps;           /**< Target fps */
    ecs_ftime_t frame_time_total;     /**< Total time spent processing a frame */
    ecs_ftime_t system_time_total;    /**< Total time spent in systems */
    ecs_ftime_t emit_time_total;      /**< Total time spent notifying observers */
    ecs_ftime_t merge_time_total;     /**< Total time spent in merges */
    ecs_ftime_t world_time_total;     /**< Time elapsed in simulation */
    ecs_ftime_t world_time_total_raw; /**< Time elapsed in simulation (no scaling) */
    ecs_ftime_t rematch_time_total;   /**< Time spent on query rematching */
    
    int64_t frame_count_total;        /**< Total number of frames */
    int64_t merge_count_total;        /**< Total number of merges */
    int64_t rematch_count_total;      /**< Total number of rematches */

    int64_t id_create_total;          /**< Total number of times a new id was created */
    int64_t id_delete_total;          /**< Total number of times an id was deleted */
    int64_t table_create_total;       /**< Total number of times a table was created */
    int64_t table_delete_total;       /**< Total number of times a table was deleted */
    int64_t pipeline_build_count_total; /**< Total number of pipeline builds */
    int64_t systems_ran_frame;        /**< Total number of systems ran in last frame */
    int64_t observers_ran_frame;      /**< Total number of times observer was invoked */

    int32_t tag_id_count;             /**< Number of tag (no data) ids in the world */
    int32_t component_id_count;       /**< Number of component (data) ids in the world */
    int32_t pair_id_count;            /**< Number of pair ids in the world */

    int32_t table_count;              /**< Number of tables */
    int32_t empty_table_count;        /**< Number of tables without entities */

    /* -- Command counts -- */
    struct {
        int64_t add_count;             /**< add commands processed */
        int64_t remove_count;          /**< remove commands processed */
        int64_t delete_count;          /**< delete commands processed */
        int64_t clear_count;           /**< clear commands processed */
        int64_t set_count;             /**< set commands processed */
        int64_t get_mut_count;         /**< get_mut/emplace commands processed */
        int64_t modified_count;        /**< modified commands processed */
        int64_t other_count;           /**< other commands processed */
        int64_t discard_count;         /**< commands discarded, happens when entity is no longer alive when running the command */
        int64_t batched_entity_count;  /**< entities for which commands were batched */
        int64_t batched_command_count; /**< commands batched */
    } cmd;

    const char *name_prefix;          /**< Value set by ecs_set_name_prefix. Used
                                       * to remove library prefixes of symbol
                                       * names (such as Ecs, ecs_) when 
                                       * registering them as names. */
} ecs_world_info_t;

/** Type that contains information about a query group. */
typedef struct ecs_query_group_info_t {
    int32_t match_count;  /**< How often tables have been matched/unmatched */
    int32_t table_count;  /**< Number of tables in group */
    void *ctx;            /**< Group context, returned by on_group_create */
} ecs_query_group_info_t;

/** @} */

/**
 * @defgroup builtin_components Builtin component types.
 * @brief Types that represent builtin components.
 * @{
 */

/** A (string) identifier. Used as pair with EcsName and EcsSymbol tags */
typedef struct EcsIdentifier {
    char *value;          /**< Identifier string */
    ecs_size_t length;    /**< Length of identifier */
    uint64_t hash;        /**< Hash of current value */
    uint64_t index_hash;  /**< Hash of existing record in current index */
    ecs_hashmap_t *index; /**< Current index */
} EcsIdentifier;

/** Component information. */
typedef struct EcsComponent {
    ecs_size_t size;           /**< Component size */
    ecs_size_t alignment;      /**< Component alignment */
} EcsComponent;

/** Component for storing a poly object */
typedef struct EcsPoly {
    ecs_poly_t *poly;          /**< Pointer to poly object */
} EcsPoly;

/** Target data for flattened relationships. */
typedef struct EcsTarget {
    int32_t count;
    ecs_record_t *target;
} EcsTarget;

/** Component for iterable entities */
typedef ecs_iterable_t EcsIterable;

/** @} */
/** @} */

/* Only include deprecated definitions if deprecated addon is required */
#ifdef FLECS_DEPRECATED
/**
 * @file addons/deprecated.h
 * @brief The deprecated addon contains deprecated operations.
 */

#ifdef FLECS_DEPRECATED

#ifndef FLECS_DEPRECATED_H
#define FLECS_DEPRECATED_H



#endif

#endif

#endif

/**
 * @defgroup api_constants API Constants
 * @brief Public API constants.
 * @{
 */

/**
 * @defgroup id_flags Component id flags.
 * @brief Id flags are bits that can be set on an id (ecs_id_t).
 * @{
 */

/** Indicates that the id is a pair. */
FLECS_API extern const ecs_id_t ECS_PAIR;

/** Automatically override component when it is inherited */
FLECS_API extern const ecs_id_t ECS_OVERRIDE;

/** Adds bitset to storage which allows component to be enabled/disabled */
FLECS_API extern const ecs_id_t ECS_TOGGLE;

/** Include all components from entity to which AND is applied */
FLECS_API extern const ecs_id_t ECS_AND;

/** @} */

/**
 * @defgroup builtin_tags Builtin component ids.
 * @{
 */

/* Builtin component ids */
FLECS_API extern const ecs_entity_t ecs_id(EcsComponent);
FLECS_API extern const ecs_entity_t ecs_id(EcsIdentifier);
FLECS_API extern const ecs_entity_t ecs_id(EcsIterable);
FLECS_API extern const ecs_entity_t ecs_id(EcsPoly);

FLECS_API extern const ecs_entity_t EcsQuery;
FLECS_API extern const ecs_entity_t EcsObserver;

/* System module component ids */
FLECS_API extern const ecs_entity_t EcsSystem;
FLECS_API extern const ecs_entity_t ecs_id(EcsTickSource);

/* Pipeline module component ids */
FLECS_API extern const ecs_entity_t ecs_id(EcsPipelineQuery);

/* Timer module component ids */
FLECS_API extern const ecs_entity_t ecs_id(EcsTimer);
FLECS_API extern const ecs_entity_t ecs_id(EcsRateFilter);

/** Root scope for builtin flecs entities */
FLECS_API extern const ecs_entity_t EcsFlecs;

/** Core module scope */
FLECS_API extern const ecs_entity_t EcsFlecsCore;

/** Entity associated with world (used for "attaching" components to world) */
FLECS_API extern const ecs_entity_t EcsWorld;

/** Wildcard entity ("*"). Matches any id, returns all matches. */
FLECS_API extern const ecs_entity_t EcsWildcard;

/** Any entity ("_"). Matches any id, returns only the first. */
FLECS_API extern const ecs_entity_t EcsAny;

/** This entity. Default source for queries. */
FLECS_API extern const ecs_entity_t EcsThis;

/** Variable entity ("$"). Used in expressions to prefix variable names */
FLECS_API extern const ecs_entity_t EcsVariable;

/** Marks a relationship as transitive. 
 * Behavior: 
 *   if R(X, Y) and R(Y, Z) then R(X, Z)
 */
FLECS_API extern const ecs_entity_t EcsTransitive;

/** Marks a relatoinship as reflexive.
 * Behavior: 
 *   R(X, X) == true
 */
FLECS_API extern const ecs_entity_t EcsReflexive;

/** Ensures that entity/component cannot be used as target in IsA relationship.
 * Final can improve the performance of rule-based queries, as they will not 
 * attempt to substitute a final component with its subsets.
 * 
 * Behavior: 
 *   if IsA(X, Y) and Final(Y) throw error
 */
FLECS_API extern const ecs_entity_t EcsFinal;

/** Ensures that component is never inherited from an IsA target.
 * 
 * Behavior:
 *   if DontInherit(X) and X(B) and IsA(A, B) then X(A) is false.
 */
FLECS_API extern const ecs_entity_t EcsDontInherit;

/** Ensures a component is always overridden.
 * 
 * Behavior:
 *   As if the component is added together with OVERRIDE | T
 */
FLECS_API extern const ecs_entity_t EcsAlwaysOverride;

/** Marks relationship as commutative.
 * Behavior:
 *   if R(X, Y) then R(Y, X)
 */
FLECS_API extern const ecs_entity_t EcsSymmetric;

/** Can be added to relationship to indicate that the relationship can only occur
 * once on an entity. Adding a 2nd instance will replace the 1st. 
 *
 * Behavior:
 *   R(X, Y) + R(X, Z) = R(X, Z)
 */
FLECS_API extern const ecs_entity_t EcsExclusive;

/** Marks a relationship as acyclic. Acyclic relationships may not form cycles. */
FLECS_API extern const ecs_entity_t EcsAcyclic;

/** Marks a relationship as traversable. Traversable relationships may be 
 * traversed with "up" queries. Traversable relationships are acyclic. */
FLECS_API extern const ecs_entity_t EcsTraversable;

/** Ensure that a component always is added together with another component.
 * 
 * Behavior:
 *   If With(R, O) and R(X) then O(X)
 *   If With(R, O) and R(X, Y) then O(X, Y)
 */
FLECS_API extern const ecs_entity_t EcsWith;

/** Ensure that relationship target is child of specified entity.
 * 
 * Behavior:
 *   If OneOf(R, O) and R(X, Y), Y must be a child of O
 *   If OneOf(R) and R(X, Y), Y must be a child of R
 */
FLECS_API extern const ecs_entity_t EcsOneOf;

/** Can be added to relationship to indicate that it should never hold data, 
 * even when it or the relationship target is a component. */
FLECS_API extern const ecs_entity_t EcsTag;

/** Tag to indicate that relationship is stored as union. Union relationships 
 * enable changing the target of a union without switching tables. Union 
 * relationships are also marked as exclusive. */
FLECS_API extern const ecs_entity_t EcsUnion;

/** Tag to indicate name identifier */
FLECS_API extern const ecs_entity_t EcsName;

/** Tag to indicate symbol identifier */
FLECS_API extern const ecs_entity_t EcsSymbol;

/** Tag to indicate alias identifier */
FLECS_API extern const ecs_entity_t EcsAlias;

/** Used to express parent-child relationships. */
FLECS_API extern const ecs_entity_t EcsChildOf;

/** Used to express inheritance relationships. */
FLECS_API extern const ecs_entity_t EcsIsA;

/** Used to express dependency relationships */
FLECS_API extern const ecs_entity_t EcsDependsOn;

/** Used to express a slot (used with prefab inheritance) */
FLECS_API extern const ecs_entity_t EcsSlotOf;

/** Tag added to module entities */
FLECS_API extern const ecs_entity_t EcsModule;

/** Tag to indicate an entity/component/system is private to a module */
FLECS_API extern const ecs_entity_t EcsPrivate;

/** Tag added to prefab entities. Any entity with this tag is automatically
 * ignored by queries, unless EcsPrefab is explicitly queried for. */
FLECS_API extern const ecs_entity_t EcsPrefab;

/** When this tag is added to an entity it is skipped by queries, unless 
 * EcsDisabled is explicitly queried for. */
FLECS_API extern const ecs_entity_t EcsDisabled;

/** Event that triggers when an id is added to an entity */
FLECS_API extern const ecs_entity_t EcsOnAdd;

/** Event that triggers when an id is removed from an entity */
FLECS_API extern const ecs_entity_t EcsOnRemove;

/** Event that triggers when a component is set for an entity */
FLECS_API extern const ecs_entity_t EcsOnSet;

/** Event that triggers when a component is unset for an entity */
FLECS_API extern const ecs_entity_t EcsUnSet;

/** Event that triggers observer when an entity starts/stops matching a query */
FLECS_API extern const ecs_entity_t EcsMonitor;

/** Event that triggers when a table is created. */
FLECS_API extern const ecs_entity_t EcsOnTableCreate;

/** Event that triggers when a table is deleted. */
FLECS_API extern const ecs_entity_t EcsOnTableDelete;

/** Event that triggers when a table becomes empty (doesn't emit on creation). */
FLECS_API extern const ecs_entity_t EcsOnTableEmpty;

/** Event that triggers when a table becomes non-empty. */
FLECS_API extern const ecs_entity_t EcsOnTableFill;

/** Relationship used for specifying cleanup behavior. */
FLECS_API extern const ecs_entity_t EcsOnDelete;

/** Relationship used to define what should happen when a target entity (second
 * element of a pair) is deleted. */
FLECS_API extern const ecs_entity_t EcsOnDeleteTarget;

/** Remove cleanup policy. Must be used as target in pair with EcsOnDelete or
 * EcsOnDeleteTarget. */
FLECS_API extern const ecs_entity_t EcsRemove;

/** Delete cleanup policy. Must be used as target in pair with EcsOnDelete or
 * EcsOnDeleteTarget. */
FLECS_API extern const ecs_entity_t EcsDelete;

/** Panic cleanup policy. Must be used as target in pair with EcsOnDelete or
 * EcsOnDeleteTarget. */
FLECS_API extern const ecs_entity_t EcsPanic;

/** Component that stores data for flattened relationships */
FLECS_API extern const ecs_entity_t ecs_id(EcsTarget);

/** Tag added to root entity to indicate its subtree should be flattened. Used
 * together with assemblies. */
FLECS_API extern const ecs_entity_t EcsFlatten;

/** Used like (EcsDefaultChildComponent, Component). When added to an entity,
 * this informs serialization formats which component to use when a value is
 * assigned to an entity without specifying the component. This is intended as
 * a hint, serialization formats are not required to use it. Adding this 
 * component does not change the behavior of core ECS operations. */
FLECS_API extern const ecs_entity_t EcsDefaultChildComponent;

/* Builtin predicates for comparing entity ids in queries. Only supported by rules */
FLECS_API extern const ecs_entity_t EcsPredEq;
FLECS_API extern const ecs_entity_t EcsPredMatch;
FLECS_API extern const ecs_entity_t EcsPredLookup;

/* Builtin marker entities for opening/closing query scopes */
FLECS_API extern const ecs_entity_t EcsScopeOpen;
FLECS_API extern const ecs_entity_t EcsScopeClose;

/** Tag used to indicate query is empty */
FLECS_API extern const ecs_entity_t EcsEmpty;

/* Pipeline module tags */
FLECS_API extern const ecs_entity_t ecs_id(EcsPipeline);
FLECS_API extern const ecs_entity_t EcsOnStart;
FLECS_API extern const ecs_entity_t EcsPreFrame;
FLECS_API extern const ecs_entity_t EcsOnLoad;
FLECS_API extern const ecs_entity_t EcsPostLoad;
FLECS_API extern const ecs_entity_t EcsPreUpdate;
FLECS_API extern const ecs_entity_t EcsOnUpdate;
FLECS_API extern const ecs_entity_t EcsOnValidate;
FLECS_API extern const ecs_entity_t EcsPostUpdate;
FLECS_API extern const ecs_entity_t EcsPreStore;
FLECS_API extern const ecs_entity_t EcsOnStore;
FLECS_API extern const ecs_entity_t EcsPostFrame;
FLECS_API extern const ecs_entity_t EcsPhase;

/** Value used to quickly check if component is builtin. This is used to quickly
 * filter out tables with builtin components (for example for ecs_delete) */
#define EcsLastInternalComponentId (ecs_id(EcsPoly))

/** The first user-defined component starts from this id. Ids up to this number
 * are reserved for builtin components */
#define EcsFirstUserComponentId (8)

/** The first user-defined entity starts from this id. Ids up to this number
 * are reserved for builtin entities */
#define EcsFirstUserEntityId (FLECS_HI_COMPONENT_ID + 128)

/* When visualized the reserved id ranges look like this:
 * [1..8]: Builtin components
 * [9..FLECS_HI_COMPONENT_ID]: Low ids reserved for application components
 * [FLECS_HI_COMPONENT_ID + 1..EcsFirstUserEntityId]: Builtin entities
 */

/** @} */
/** @} */

/**
 * @defgroup world_api World
 * @brief Functions for working with `ecs_world_t`.
 * @{
 */

/**
 * @defgroup world_creation_deletion Creation & Deletion
 * @{
 */

/** Create a new world.
 * This operation automatically imports modules from addons Flecs has been built
 * with, except when the module specifies otherwise.
 *
 * @return A new world
 */
FLECS_API
ecs_world_t* ecs_init(void);

/** Create a new world with just the core module.
 * Same as ecs_init, but doesn't import modules from addons. This operation is
 * faster than ecs_init and results in less memory utilization.
 *
 * @return A new tiny world
 */
FLECS_API
ecs_world_t* ecs_mini(void);

/** Create a new world with arguments.
 * Same as ecs_init, but allows passing in command line arguments. Command line
 * arguments are used to:
 * - automatically derive the name of the application from argv[0]
 *
 * @return A new world
 */
FLECS_API
ecs_world_t* ecs_init_w_args(
    int argc,
    char *argv[]);

/** Delete a world.
 * This operation deletes the world, and everything it contains.
 *
 * @param world The world to delete.
 * @return Zero if successful, non-zero if failed.
 */
FLECS_API
int ecs_fini(
    ecs_world_t *world);

/** Returns whether the world is being deleted.
 * This operation can be used in callbacks like type hooks or observers to 
 * detect if they are invoked while the world is being deleted.
 *
 * @param world The world.
 * @return True if being deleted, false if not.
 */
FLECS_API
bool ecs_is_fini(
    const ecs_world_t *world);

/** Register action to be executed when world is destroyed.
 * Fini actions are typically used when a module needs to clean up before a
 * world shuts down.
 * 
 * @param world The world.
 * @param action The function to execute.
 * @param ctx Userdata to pass to the function */
FLECS_API
void ecs_atfini(
    ecs_world_t *world,
    ecs_fini_action_t action,
    void *ctx);

/** @} */

/**
 * @defgroup world_frame Frame functions
 * @{
 */

/** Begin frame. 
 * When an application does not use ecs_progress to control the main loop, it
 * can still use Flecs features such as FPS limiting and time measurements. This
 * operation needs to be invoked whenever a new frame is about to get processed.
 *
 * Calls to ecs_frame_begin must always be followed by ecs_frame_end.
 *
 * The function accepts a delta_time parameter, which will get passed to 
 * systems. This value is also used to compute the amount of time the function
 * needs to sleep to ensure it does not exceed the target_fps, when it is set.
 * When 0 is provided for delta_time, the time will be measured.
 *
 * This function should only be ran from the main thread.
 *
 * @param world The world.
 * @param delta_time Time elapsed since the last frame.
 * @return The provided delta_time, or measured time if 0 was provided.
 */
FLECS_API
ecs_ftime_t ecs_frame_begin(
    ecs_world_t *world,
    ecs_ftime_t delta_time);

/** End frame. 
 * This operation must be called at the end of the frame, and always after
 * ecs_frame_begin.
 *
 * @param world The world.
 */
FLECS_API
void ecs_frame_end(
    ecs_world_t *world);

/** Register action to be executed once after frame.
 * Post frame actions are typically used for calling operations that cannot be
 * invoked during iteration, such as changing the number of threads.
 * 
 * @param world The world.
 * @param action The function to execute.
 * @param ctx Userdata to pass to the function */
FLECS_API
void ecs_run_post_frame(
    ecs_world_t *world,
    ecs_fini_action_t action,
    void *ctx);    

/** Signal exit
 * This operation signals that the application should quit. It will cause
 * ecs_progress to return false.
 *
 * @param world The world to quit.
 */
FLECS_API
void ecs_quit(
    ecs_world_t *world);

/** Return whether a quit has been signaled.
 *
 * @param world The world.
 */
FLECS_API 
bool ecs_should_quit(
    const ecs_world_t *world);

/** Measure frame time. 
 * Frame time measurements measure the total time passed in a single frame, and 
 * how much of that time was spent on systems and on merging.
 *
 * Frame time measurements add a small constant-time overhead to an application.
 * When an application sets a target FPS, frame time measurements are enabled by
 * default.
 *
 * @param world The world.
 * @param enable Whether to enable or disable frame time measuring.
 */
FLECS_API void ecs_measure_frame_time(
    ecs_world_t *world,
    bool enable);

/** Measure system time. 
 * System time measurements measure the time spent in each system.
 *
 * System time measurements add overhead to every system invocation and 
 * therefore have a small but measurable impact on application performance.
 * System time measurements must be enabled before obtaining system statistics.
 *
 * @param world The world.
 * @param enable Whether to enable or disable system time measuring.
 */
FLECS_API void ecs_measure_system_time(
    ecs_world_t *world,
    bool enable);   

/** Set target frames per second (FPS) for application.
 * Setting the target FPS ensures that ecs_progress is not invoked faster than
 * the specified FPS. When enabled, ecs_progress tracks the time passed since
 * the last invocation, and sleeps the remaining time of the frame (if any).
 *
 * This feature ensures systems are ran at a consistent interval, as well as
 * conserving CPU time by not running systems more often than required.
 *
 * Note that ecs_progress only sleeps if there is time left in the frame. Both
 * time spent in flecs as time spent outside of flecs are taken into
 * account.
 *
 * @param world The world.
 * @param fps The target FPS.
 */
FLECS_API
void ecs_set_target_fps(
    ecs_world_t *world,
    ecs_ftime_t fps);

/** @} */

/**
 * @defgroup commands Commands
 * @{
 */

/** Begin readonly mode.
 * Readonly mode guarantees that no mutations will occur on the world, which
 * makes the world safe to access from multiple threads. While the world is in
 * readonly mode, operations are deferred.
 * 
 * Note that while similar to ecs_defer_begin, deferring only does not guarantee
 * the world is not mutated. Operations that are not deferred (like creating a
 * query) update data structures on the world and are allowed when deferring is
 * enabled, but not when the world is in readonly mode.
 * 
 * A call to ecs_readonly_begin must be followed up with ecs_readonly_end.
 * 
 * The ecs_progress() function automatically enables readonly mode while systems
 * are executed.
 * 
 * When a world has more than one stage, the specific stage must be provided to
 * mutating ECS operations. Failing to do so will throw a readonly assert. A
 * world typically has more than one stage when using threads. An example:
 * 
 * ecs_set_stage_count(world, 2);
 * ecs_stage_t *stage = ecs_get_stage(world, 1);
 * 
 * ecs_readonly_begin(world);
 * ecs_add(world, e, Tag); // readonly assert
 * ecs_add(stage, e, Tag); // OK
 *
 * @param world The world
 * @return Whether world is in readonly mode.
 */
FLECS_API
bool ecs_readonly_begin(
    ecs_world_t *world);

/** End readonly mode.
 * This operation ends readonly mode, and must be called after 
 * ecs_readonly_begin. Operations that were deferred while the world was in
 * readonly mode will be flushed.
 *
 * @param world The world
 */
FLECS_API
void ecs_readonly_end(
    ecs_world_t *world);

/** Merge world or stage.
 * When automatic merging is disabled, an application can call this
 * operation on either an individual stage, or on the world which will merge
 * all stages. This operation may only be called when staging is not enabled
 * (either after progress() or after readonly_end()).
 *
 * This operation may be called on an already merged stage or world.
 *
 * @param world The world.
 */
FLECS_API
void ecs_merge(
    ecs_world_t *world);

/** Defer operations until end of frame. 
 * When this operation is invoked while iterating, operations inbetween the
 * defer_begin and defer_end operations are executed at the end of the frame.
 *
 * This operation is thread safe.
 * 
 * @param world The world.
 * @return true if world changed from non-deferred mode to deferred mode.
 */
FLECS_API
bool ecs_defer_begin(
    ecs_world_t *world);

/** Test if deferring is enabled for current stage.
 * 
 * @param world The world.
 * @return True if deferred, false if not.
 */
FLECS_API
bool ecs_is_deferred(
    const ecs_world_t *world);

/** End block of operations to defer. 
 * See defer_begin.
 *
 * This operation is thread safe.
 *
 * @param world The world.
 * @return true if world changed from deferred mode to non-deferred mode.
 */
FLECS_API
bool ecs_defer_end(
    ecs_world_t *world);

/** Suspend deferring but do not flush queue.
 * This operation can be used to do an undeferred operation while not flushing
 * the operations in the queue.
 * 
 * An application should invoke ecs_defer_resume before ecs_defer_end is called.
 * The operation may only be called when deferring is enabled.
 * 
 * @param world The world.
 */
FLECS_API
void ecs_defer_suspend(
    ecs_world_t *world);

/** Resume deferring.
 * See ecs_defer_suspend.
 * 
 * @param world The world.
 */
FLECS_API
void ecs_defer_resume(
    ecs_world_t *world);

/** Enable/disable automerging for world or stage.
 * When automerging is enabled, staged data will automatically be merged with
 * the world when staging ends. This happens at the end of progress(), at a
 * sync point or when readonly_end() is called.
 *
 * Applications can exercise more control over when data from a stage is merged
 * by disabling automerging. This requires an application to explicitly call
 * merge() on the stage.
 *
 * When this function is invoked on the world, it sets all current stages to
 * the provided value and sets the default for new stages. When this function is
 * invoked on a stage, automerging is only set for that specific stage.
 *
 * @param world The world.
 * @param automerge Whether to enable or disable automerging.
 */
FLECS_API
void ecs_set_automerge(
    ecs_world_t *world,
    bool automerge);

/** Configure world to have N stages.
 * This initializes N stages, which allows applications to defer operations to
 * multiple isolated defer queues. This is typically used for applications with
 * multiple threads, where each thread gets its own queue, and commands are
 * merged when threads are synchronized.
 *
 * Note that the ecs_set_threads function already creates the appropriate
 * number of stages. The set_stage_count() operation is useful for applications that
 * want to manage their own stages and/or threads.
 * 
 * @param world The world.
 * @param stages The number of stages.
 */
FLECS_API
void ecs_set_stage_count(
    ecs_world_t *world,
    int32_t stages);

/** Get number of configured stages.
 * Return number of stages set by ecs_set_stage_count.
 *
 * @param world The world.
 * @return The number of stages used for threading.
 */
FLECS_API
int32_t ecs_get_stage_count(
    const ecs_world_t *world);

/** Get current stage id.
 * The stage id can be used by an application to learn about which stage it is
 * using, which typically corresponds with the worker thread id.
 *
 * @param world The world.
 * @return The stage id.
 */
FLECS_API
int32_t ecs_get_stage_id(
    const ecs_world_t *world);

/** Get stage-specific world pointer.
 * Flecs threads can safely invoke the API as long as they have a private 
 * context to write to, also referred to as the stage. This function returns a
 * pointer to a stage, disguised as a world pointer.
 *
 * Note that this function does not(!) create a new world. It simply wraps the
 * existing world in a thread-specific context, which the API knows how to
 * unwrap. The reason the stage is returned as an ecs_world_t is so that it
 * can be passed transparently to the existing API functions, vs. having to 
 * create a dediated API for threading.
 *
 * @param world The world.
 * @param stage_id The index of the stage to retrieve.
 * @return A thread-specific pointer to the world. 
 */
FLECS_API
ecs_world_t* ecs_get_stage(
    const ecs_world_t *world,
    int32_t stage_id);

/** Test whether the current world is readonly.
 * This function allows the code to test whether the currently used world
 * is readonly or whether it allows for writing.  
 *
 * @param world A pointer to a stage or the world.
 * @return True if the world or stage is readonly.
 */
FLECS_API
bool ecs_stage_is_readonly(
    const ecs_world_t *world);

/** Create asynchronous stage.
 * An asynchronous stage can be used to asynchronously queue operations for
 * later merging with the world. An asynchronous stage is similar to a regular
 * stage, except that it does not allow reading from the world.
 *
 * Asynchronous stages are never merged automatically, and must therefore be
 * manually merged with the ecs_merge function. It is not necessary to call
 * defer_begin or defer_end before and after enqueuing commands, as an 
 * asynchronous stage unconditionally defers operations.
 *
 * The application must ensure that no commands are added to the stage while the
 * stage is being merged.
 *
 * An asynchronous stage must be cleaned up by ecs_async_stage_free. 
 *
 * @param world The world.
 * @return The stage.
 */
FLECS_API
ecs_world_t* ecs_async_stage_new(
    ecs_world_t *world);

/** Free asynchronous stage.
 * The provided stage must be an asynchronous stage. If a non-asynchronous stage
 * is provided, the operation will fail.
 *
 * @param stage The stage to free.
 */
FLECS_API
void ecs_async_stage_free(
    ecs_world_t *stage);

/** Test whether provided stage is asynchronous.
 *
 * @param stage The stage.
 * @return True when the stage is asynchronous, false for a regular stage or 
 *         world.
 */
FLECS_API
bool ecs_stage_is_async(
    ecs_world_t *stage);

/** @} */

/**
 * @defgroup world_misc Misc
 * @{
 */

/** Set a world context.
 * This operation allows an application to register custom data with a world
 * that can be accessed anywhere where the application has the world.
 *
 * @param world The world.
 * @param ctx A pointer to a user defined structure.
 * @param ctx_free A function that is invoked with ctx when the world is freed.
 */
FLECS_API
void ecs_set_ctx(
    ecs_world_t *world,
    void *ctx,
    ecs_ctx_free_t ctx_free);

/** Set a world binding context.
 * Same as ecs_set_ctx but for binding context. A binding context is intended
 * specifically for language bindings to store binding specific data.
 *
 * @param world The world.
 * @param ctx A pointer to a user defined structure.
 * @param ctx_free A function that is invoked with ctx when the world is freed.
 */
FLECS_API
void ecs_set_binding_ctx(
    ecs_world_t *world,
    void *ctx,
    ecs_ctx_free_t ctx_free);

/** Get the world context.
 * This operation retrieves a previously set world context.
 *
 * @param world The world.
 * @return The context set with ecs_set_ctx. If no context was set, the
 *         function returns NULL.
 */
FLECS_API
void* ecs_get_ctx(
    const ecs_world_t *world);

/** Get the world binding context.
 * This operation retrieves a previously set world binding context.
 *
 * @param world The world.
 * @return The context set with ecs_set_binding_ctx. If no context was set, the
 *         function returns NULL.
 */
FLECS_API
void* ecs_get_binding_ctx(
    const ecs_world_t *world);

/** Get world info.
 *
 * @param world The world.
 * @return Pointer to the world info. Valid for as long as the world exists.
 */
FLECS_API
const ecs_world_info_t* ecs_get_world_info(
    const ecs_world_t *world);

/** Dimension the world for a specified number of entities.
 * This operation will preallocate memory in the world for the specified number
 * of entities. Specifying a number lower than the current number of entities in
 * the world will have no effect.
 *
 * @param world The world.
 * @param entity_count The number of entities to preallocate.
 */
FLECS_API
void ecs_dim(
    ecs_world_t *world,
    int32_t entity_count);

/** Set a range for issueing new entity ids.
 * This function constrains the entity identifiers returned by ecs_new to the 
 * specified range. This operation can be used to ensure that multiple processes
 * can run in the same simulation without requiring a central service that
 * coordinates issueing identifiers.
 * 
 * If id_end is set to 0, the range is infinite. If id_end is set to a non-zero
 * value, it has to be larger than id_start. If id_end is set and ecs_new is
 * invoked after an id is issued that is equal to id_end, the application will
 * abort.
 * 
 * @param world The world.
 * @param id_start The start of the range.
 * @param id_end The end of the range.
 */
FLECS_API
void ecs_set_entity_range(
    ecs_world_t *world,
    ecs_entity_t id_start,
    ecs_entity_t id_end);

/** Enable/disable range limits.
 * When an application is both a receiver of range-limited entities and a
 * producer of range-limited entities, range checking needs to be temporarily
 * disabled when inserting received entities. Range checking is disabled on a 
 * stage, so setting this value is thread safe.
 *
 * @param world The world.
 * @param enable True if range checking should be enabled, false to disable.
 * @return The previous value.
 */
FLECS_API
bool ecs_enable_range_check(
    ecs_world_t *world,
    bool enable);

/** Get the largest issued entity id (not counting generation).
 * 
 * @param world The world.
 */
FLECS_API
ecs_entity_t ecs_get_max_id(
    const ecs_world_t *world);

/** Force aperiodic actions.
 * The world may delay certain operations until they are necessary for the
 * application to function correctly. This may cause observable side effects
 * such as delayed triggering of events, which can be inconvenient when for
 * example running a test suite.
 * 
 * The flags parameter specifies which aperiodic actions to run. Specify 0 to
 * run all actions. Supported flags start with 'EcsAperiodic'. Flags identify
 * internal mechanisms and may change unannounced.
 * 
 * @param world The world.
 * @param flags The flags specifying which actions to run.
 */
FLECS_API
void ecs_run_aperiodic(
    ecs_world_t *world,
    ecs_flags32_t flags);

/** Cleanup empty tables.
 * This operation cleans up empty tables that meet certain conditions. Having
 * large amounts of empty tables does not negatively impact performance of the
 * ECS, but can take up considerable amounts of memory, especially in 
 * applications with many components, and many components per entity.
 * 
 * The generation specifies the minimum number of times this operation has
 * to be called before an empty table is cleaned up. If a table becomes non
 * empty, the generation is reset.
 * 
 * The operation allows for both a "clear" generation and a "delete"
 * generation. When the clear generation is reached, the table's 
 * resources are freed (like component arrays) but the table itself is not
 * deleted. When the delete generation is reached, the empty table is deleted.
 * 
 * By specifying a non-zero id the cleanup logic can be limited to tables with
 * a specific (component) id. The operation will only increase the generation
 * count of matching tables.
 * 
 * The min_id_count specifies a lower bound for the number of components a table
 * should have. Often the more components a table has, the more specific it is 
 * and therefore less likely to be reused.
 * 
 * The time budget specifies how long the operation should take at most.
 * 
 * @param world The world.
 * @param id Optional component filter for the tables to evaluate.
 * @param clear_generation Free table data when generation > clear_generation.
 * @param delete_generation Delete table when generation > delete_generation.
 * @param min_id_count Minimum number of component ids the table should have.
 * @param time_budget_seconds Amount of time operation is allowed to spend.
 * @return Number of deleted tables.
 */
FLECS_API
int32_t ecs_delete_empty_tables(
    ecs_world_t *world,
    ecs_id_t id,
    uint16_t clear_generation,
    uint16_t delete_generation,
    int32_t min_id_count,
    double time_budget_seconds);

/** Get world from poly.
 *
 * @param poly A pointer to a poly object.
 * @return The world.
 */
FLECS_API
const ecs_world_t* ecs_get_world(
    const ecs_poly_t *poly);

/** Get entity from poly.
 *
 * @param poly A pointer to a poly object.
 * @return Entity associated with the poly object.
 */
FLECS_API
ecs_entity_t ecs_get_entity(
    const ecs_poly_t *poly);

/** Test if pointer is of specified type.
 * Usage:
 *   ecs_poly_is(ptr, ecs_world_t)
 * 
 * This operation only works for poly types.
 * 
 * @param object The object to test.
 * @param type The id of the type.
 * @return True if the pointer is of the specified type.
 */
FLECS_API
bool ecs_poly_is_(
    const ecs_poly_t *object,
    int32_t type);

#define ecs_poly_is(object, type)\
    ecs_poly_is_(object, type##_magic)

/** Make a pair id.
 * This function is equivalent to using the ecs_pair macro, and is added for
 * convenience to make it easier for non C/C++ bindings to work with pairs.
 *
 * @param first The first element of the pair of the pair.
 * @param second The target of the pair.
 */
FLECS_API
ecs_id_t ecs_make_pair(
    ecs_entity_t first,
    ecs_entity_t second);

/** @} */

/** @} */

/**
 * @defgroup entities Entities
 * @brief Functions for working with `ecs_entity_t`.
 * @{
 */

/**
 * @defgroup creating_entities Creating & Deleting
 * @brief Functions for creating and deleting entities.
 * @{
 */

/** Create new entity id.
 * This operation returns an unused entity id. This operation is guaranteed to
 * return an empty entity as it does not use values set by ecs_set_scope or
 * ecs_set_with.
 *
 * @param world The world.
 * @return The new entity id.
 */
FLECS_API
ecs_entity_t ecs_new_id(
    ecs_world_t *world);

/** Create new low id.
 * This operation returns a new low id. Entity ids start after the
 * FLECS_HI_COMPONENT_ID constant. This reserves a range of low ids for things 
 * like components, and allows parts of the code to optimize operations.
 *
 * Note that FLECS_HI_COMPONENT_ID does not represent the maximum number of 
 * components that can be created, only the maximum number of components that
 * can take advantage of these optimizations.
 * 
 * This operation is guaranteed to return an empty entity as it does not use 
 * values set by ecs_set_scope or ecs_set_with.
 * 
 * This operation does not recycle ids.
 *
 * @param world The world.
 * @return The new component id.
 */
FLECS_API
ecs_entity_t ecs_new_low_id(
    ecs_world_t *world);

/** Create new entity with (component) id.
 * This operation creates a new entity with an optional (component) id. When 0
 * is passed to the id parameter, no component is added to the new entity.
 * 
 * @param world The world.
 * @param id The component id to initialize the new entity with.
 * @return The new entity.
 */
FLECS_API
ecs_entity_t ecs_new_w_id(
    ecs_world_t *world,
    ecs_id_t id);

/** Create new entity in table.
 * This operation creates a new entity in the specified table.
 * 
 * @param world The world.
 * @param table The table to which to add the new entity.
 * @return The new entity.
 */
FLECS_API
ecs_entity_t ecs_new_w_table(
    ecs_world_t *world,
    ecs_table_t *table);

/** Find or create an entity. 
 * This operation creates a new entity, or modifies an existing one. When a name
 * is set in the ecs_entity_desc_t::name field and ecs_entity_desc_t::entity is
 * not set, the operation will first attempt to find an existing entity by that
 * name. If no entity with that name can be found, it will be created.
 *
 * If both a name and entity handle are provided, the operation will check if
 * the entity name matches with the provided name. If the names do not match,
 * the function will fail and return 0.
 *
 * If an id to a non-existing entity is provided, that entity id become alive.
 * 
 * See the documentation of ecs_entity_desc_t for more details. 
 *
 * @param world The world.
 * @param desc Entity init parameters.
 * @return A handle to the new or existing entity, or 0 if failed.
 */
FLECS_API
ecs_entity_t ecs_entity_init(
    ecs_world_t *world,
    const ecs_entity_desc_t *desc);

/** Bulk create/populate new entities.
 * This operation bulk inserts a list of new or predefined entities into a
 * single table.
 * 
 * The operation does not take ownership of component arrays provided by the
 * application. Components that are non-trivially copyable will be moved into
 * the storage.
 * 
 * The operation will emit OnAdd events for each added id, and OnSet events for
 * each component that has been set.
 * 
 * If no entity ids are provided by the application, the returned array of ids
 * points to an internal datastructure which changes when new entities are
 * created/deleted.
 * 
 * If as a result of the operation triggers are invoked that deletes
 * entities and no entity ids were provided by the application, the returned
 * array of identifiers may be incorrect. To avoid this problem, an application
 * can first call ecs_bulk_init to create empty entities, copy the array to one
 * that is owned by the application, and then use this array to populate the
 * entities.
 * 
 * @param world The world.
 * @param desc Bulk creation parameters.
 * @return Array with the list of entity ids created/populated.
 */
FLECS_API
const ecs_entity_t* ecs_bulk_init(
    ecs_world_t *world,
    const ecs_bulk_desc_t *desc);

/** Create N new entities.
 * This operation is the same as ecs_new_w_id, but creates N entities
 * instead of one.
 * 
 * @param world The world.
 * @param id The component id to create the entities with.
 * @param count The number of entities to create.
 * @return The first entity id of the newly created entities.
 */
FLECS_API
const ecs_entity_t* ecs_bulk_new_w_id(
    ecs_world_t *world,
    ecs_id_t id,
    int32_t count);

/** Clone an entity
 * This operation clones the components of one entity into another entity. If
 * no destination entity is provided, a new entity will be created. Component
 * values are not copied unless copy_value is true.
 * 
 * If the source entity has a name, it will not be copied to the destination
 * entity. This is to prevent having two entities with the same name under the
 * same parent, which is not allowed.
 *
 * @param world The world.
 * @param dst The entity to copy the components to.
 * @param src The entity to copy the components from.
 * @param copy_value If true, the value of components will be copied to dst.
 * @return The destination entity.
 */
FLECS_API
ecs_entity_t ecs_clone(
    ecs_world_t *world,
    ecs_entity_t dst,
    ecs_entity_t src,
    bool copy_value);

/** Delete an entity.
 * This operation will delete an entity and all of its components. The entity id
 * will be made available for recycling. If the entity passed to ecs_delete is
 * not alive, the operation will have no side effects.
 *
 * @param world The world.
 * @param entity The entity.
 */
FLECS_API
void ecs_delete(
    ecs_world_t *world,
    ecs_entity_t entity);

/** Delete all entities with the specified id.
 * This will delete all entities (tables) that have the specified id. The id 
 * may be a wildcard and/or a pair.
 * 
 * @param world The world.
 * @param id The id.
 */
FLECS_API
void ecs_delete_with(
    ecs_world_t *world,
    ecs_id_t id);

/** @} */

/**
 * @defgroup adding_removing Adding & Removing
 * @brief Functions for adding and removing components.
 * @{
 */

/** Add a (component) id to an entity.
 * This operation adds a single (component) id to an entity. If the entity 
 * already has the id, this operation will have no side effects.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id to add.
 */
FLECS_API
void ecs_add_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Remove a (component) id from an entity.
 * This operation removes a single (component) id to an entity. If the entity 
 * does not have the id, this operation will have no side effects.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id to remove.
 */
FLECS_API
void ecs_remove_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Add override for (component) id.
 * Adding an override to an entity ensures that when the entity is instantiated
 * (by adding an IsA relationship to it) the component with the override is
 * copied to a component that is private to the instance. By default components
 * reachable through an IsA relationship are shared.
 * 
 * Adding an override does not add the component. If an override is added to an
 * entity that does not have the component, it will still be added to the
 * instance, but with an uninitialized value (unless the component has a ctor).
 * When the entity does have the entity, the component of the instance will be
 * initialized with the value of the component on the entity.
 * 
 * This is the same as what happens when calling ecs_add_id for an id that is
 * inherited (reachable through an IsA relationship).
 * 
 * This operation is equivalent to doing:
 *   ecs_add_id(world, entity, ECS_OVERRIDE | id);
 * 
 * @param world The world.
 * @param entity The entity.
 * @param id The id to override.
 */
FLECS_API
void ecs_override_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Clear all components.
 * This operation will remove all components from an entity.
 *
 * @param world The world.
 * @param entity The entity.
 */
FLECS_API
void ecs_clear(
    ecs_world_t *world,
    ecs_entity_t entity);

/** Remove all instances of the specified (component) id.
 * This will remove the specified id from all entities (tables). The id may be
 * a wildcard and/or a pair.
 * 
 * @param world The world.
 * @param id The id.
 */
FLECS_API
void ecs_remove_all(
    ecs_world_t *world,
    ecs_id_t id);

/** Set current with id.
 * New entities are automatically created with the specified id.
 *
 * @param world The world.
 * @param id The id.
 * @return The previous id.
 */
FLECS_API
ecs_entity_t ecs_set_with(
    ecs_world_t *world,
    ecs_id_t id);

/** Get current with id.
 * Get the id set with ecs_set_with.
 *
 * @param world The world.
 * @return The last id provided to ecs_set_with.
 */
FLECS_API
ecs_id_t ecs_get_with(
    const ecs_world_t *world);

/** @} */

/**
 * @defgroup enabling_disabling Enabling & Disabling
 * @brief Functions for enabling/disabling entities and components.
 * @{
 */

/** Enable or disable entity.
 * This operation enables or disables an entity by adding or removing the
 * EcsDisabled tag. A disabled entity will not be matched with any systems,
 * unless the system explicitly specifies the EcsDisabled tag.
 *
 * @param world The world.
 * @param entity The entity to enable or disable.
 * @param enabled true to enable the entity, false to disable.
 */
FLECS_API
void ecs_enable(
    ecs_world_t *world,
    ecs_entity_t entity,
    bool enabled);

/** Enable or disable component.
 * Enabling or disabling a component does not add or remove a component from an
 * entity, but prevents it from being matched with queries. This operation can
 * be useful when a component must be temporarily disabled without destroying
 * its value. It is also a more performant operation for when an application
 * needs to add/remove components at high frequency, as enabling/disabling is
 * cheaper than a regular add or remove.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The component.
 * @param enable True to enable the component, false to disable.
 */
FLECS_API 
void ecs_enable_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id,
    bool enable);

/** Test if component is enabled.
 * Test whether a component is currently enabled or disabled. This operation
 * will return true when the entity has the component and if it has not been
 * disabled by ecs_enable_component.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The component.
 * @return True if the component is enabled, otherwise false.
 */
FLECS_API 
bool ecs_is_enabled_id(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** @} */

/**
 * @defgroup getting Getting & Setting
 * @brief Functions for getting/setting components.
 * @{
 */

/** Get an immutable pointer to a component.
 * This operation obtains a const pointer to the requested component. The
 * operation accepts the component entity id.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id of the component to get.
 * @return The component pointer, NULL if the entity does not have the component.
 */
FLECS_API
const void* ecs_get_id(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Create a component ref.
 * A ref is a handle to an entity + component which caches a small amount of
 * data to reduce overhead of repeatedly accessing the component. Use 
 * ecs_ref_get to get the component data.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id of the component.
 * @return The reference.
 */
FLECS_API
ecs_ref_t ecs_ref_init_id(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Get component from ref.
 * Get component pointer from ref. The ref must be created with ecs_ref_init.
 * 
 * @param world The world.
 * @param ref The ref.
 * @param id The component id.
 * @return The component pointer, NULL if the entity does not have the component.
 */
FLECS_API
void* ecs_ref_get_id(
    const ecs_world_t *world,
    ecs_ref_t *ref,
    ecs_id_t id);

/** Update ref.
 * Ensures contents of ref are up to date. Same as ecs_ref_get_id, but does not
 * return pointer to component id. 
 * 
 * @param world The world.
 * @param ref The ref.
 */
FLECS_API
void ecs_ref_update(
    const ecs_world_t *world,
    ecs_ref_t *ref);

/** Get a mutable pointer to a component.
 * This operation returns a mutable pointer to a component. If the component did
 * not yet exist, it will be added.
 * 
 * If get_mut is called when the world is in deferred/readonly mode, the 
 * function will:
 * - return a pointer to a temp storage if the component does not yet exist, or
 * - return a pointer to the existing component if it exists
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The entity id of the component to obtain.
 * @return The component pointer.
 */
FLECS_API
void* ecs_get_mut_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Combines get_mut + modifed in single operation. 
 * This operation is a more efficient alternative to calling ecs_get_mut_id and
 * ecs_modified_id separately. This operation is only valid when the world is in
 * deferred mode, which ensures that the Modified event is not emitted before
 * the modification takes place.
 * 
 * @param world The world.
 * @param entity The entity.
 * @param id The id of the component to obtain.
 * @return The component pointer.
 */
FLECS_API
void* ecs_get_mut_modified_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Begin exclusive write access to entity.
 * This operation provides safe exclusive access to the components of an entity
 * without the overhead of deferring operations.
 * 
 * When this operation is called simultaneously for the same entity more than
 * once it will throw an assert. Note that for this to happen, asserts must be
 * enabled. It is up to the application to ensure that access is exclusive, for
 * example by using a read-write mutex.
 * 
 * Exclusive access is enforced at the table level, so only one entity can be
 * exclusively accessed per table. The exclusive access check is thread safe.
 * 
 * This operation must be followed up with ecs_write_end.
 * 
 * @param world The world.
 * @param entity The entity.
 * @return A record to the entity.
 */
FLECS_API
ecs_record_t* ecs_write_begin(
    ecs_world_t *world,
    ecs_entity_t entity);

/** End exclusive write access to entity.
 * This operation ends exclusive access, and must be called after 
 * ecs_write_begin.
 * 
 * @param record Record to the entity.
 */
FLECS_API
void ecs_write_end(
    ecs_record_t *record);

/** Begin read access to entity.
 * This operation provides safe read access to the components of an entity.
 * Multiple simultaneous reads are allowed per entity.
 * 
 * This operation ensures that code attempting to mutate the entity's table will
 * throw an assert. Note that for this to happen, asserts must be enabled. It is
 * up to the application to ensure that this does not happen, for example by
 * using a read-write mutex.
 * 
 * This operation does *not* provide the same guarantees as a read-write mutex,
 * as it is possible to call ecs_read_begin after calling ecs_write_begin. It is
 * up to application has to ensure that this does not happen.
 * 
 * This operation must be followed up with ecs_read_end.
 *
 * @param world The world.
 * @param entity The entity.
 * @return A record to the entity.
 */
FLECS_API
const ecs_record_t* ecs_read_begin(
    ecs_world_t *world,
    ecs_entity_t entity);

/** End read access to entity.
 * This operation ends read access, and must be called after ecs_read_begin.
 *
 * @param record Record to the entity.
 */
FLECS_API
void ecs_read_end(
    const ecs_record_t *record);

/** Get entity corresponding with record.
 * This operation only works for entities that are not empty.
 * 
 * @param record The record for which to obtain the entity id.
 */
FLECS_API
ecs_entity_t ecs_record_get_entity(
    const ecs_record_t *record);

/** Get component from entity record.
 * This operation returns a pointer to a component for the entity 
 * associated with the provided record. For safe access to the component, obtain
 * the record with ecs_read_begin or ecs_write_begin.
 * 
 * Obtaining a component from a record is faster than obtaining it from the
 * entity handle, as it reduces the number of lookups required.
 * 
 * @param world The world.
 * @param record Record to the entity.
 * @param id The (component) id.
 * @return Pointer to component, or NULL if entity does not have the component.
 */
FLECS_API
const void* ecs_record_get_id(
    ecs_world_t *world,
    const ecs_record_t *record,
    ecs_id_t id);

/** Same as ecs_record_get_id, but returns a mutable pointer.
 * For safe access to the component, obtain the record with ecs_write_begin.
 * 
 * @param world The world.
 * @param record Record to the entity.
 * @param id The (component) id.
 * @return Pointer to component, or NULL if entity does not have the component.
 */
FLECS_API
void* ecs_record_get_mut_id(
    ecs_world_t *world,
    ecs_record_t *record,
    ecs_id_t id);

/** Test if entity for record has component. 
 * 
 * @param world The world.
 * @param record Record to the entity.
 * @param id The (component) id.
 */
FLECS_API
bool ecs_record_has_id(
    ecs_world_t *world,
    const ecs_record_t *record,
    ecs_id_t id);

/** Emplace a component.
 * Emplace is similar to get_mut except that the component constructor is not
 * invoked for the returned pointer, allowing the component to be "constructed"
 * directly in the storage.
 *
 * Emplace can only be used if the entity does not yet have the component. If
 * the entity has the component, the operation will fail.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The component to obtain.
 * @return The (uninitialized) component pointer.
 */
FLECS_API
void* ecs_emplace_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Signal that a component has been modified.
 * This operation is usually used after modifying a component value obtained by
 * ecs_get_mut_id. The operation will mark the component as dirty, and invoke
 * OnSet observers and hooks.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id of the component that was modified.
 */
FLECS_API 
void ecs_modified_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Set the value of a component.
 * This operation allows an application to set the value of a component. The
 * operation is equivalent to calling ecs_get_mut_id followed by 
 * ecs_modified_id. The operation will not modify the value of the passed in
 * component. If the component has a copy hook registered, it will be used to
 * copy in the component.
 *
 * If the provided entity is 0, a new entity will be created.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id of the component to set.
 * @param size The size of the pointed-to value.
 * @param ptr The pointer to the value.
 * @return The entity. A new entity if no entity was provided.
 */
FLECS_API
ecs_entity_t ecs_set_id(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id,
    size_t size,
    const void *ptr);

/** @} */

/**
 * @defgroup liveliness Entity Liveliness
 * @brief Functions for testing and modifying entity liveliness.
 * @{
 */

/** Test whether an entity is valid.
 * Entities that are valid can be used with API functions. Using invalid 
 * entities with API operations will cause the function to panic.
 *
 * An entity is valid if it is not 0 and if it is alive.
 *
 * is_valid will return true for ids that don't exist (alive or not alive). This
 * allows for using ids that have never been created by ecs_new or similar. In
 * this the function differs from ecs_is_alive, which will return false for
 * entities that do not yet exist.
 *
 * The operation will return false for an id that exists and is not alive, as
 * using this id with an API operation would cause it to assert.
 *
 * @param world The world.
 * @param e The entity.
 * @return True if the entity is valid, false if the entity is not valid.
 */
FLECS_API
bool ecs_is_valid(
    const ecs_world_t *world,
    ecs_entity_t e);

/** Test whether an entity is alive.
 * Entities are alive after they are created, and become not alive when they are
 * deleted. Operations that return alive ids are (amongst others) ecs_new_id, 
 * ecs_new_low_id and ecs_entity_init. Ids can be made alive with the ecs_ensure 
 * function.
 * 
 * After an id is deleted it can be recycled. Recycled ids are different from
 * the original id in that they have a different generation count. This makes it
 * possible for the API to distinguish between the two. An example:
 * 
 *   ecs_entity_t e1 = ecs_new_id(world);
 *   ecs_is_alive(world, e1);             // true
 *   ecs_delete(world, e1);
 *   ecs_is_alive(world, e1);             // false
 * 
 *   ecs_entity_t e2 = ecs_new_id(world); // recycles e1
 *   ecs_is_alive(world, e2);             // true
 *   ecs_is_alive(world, e1);             // false
 *
 * @param world The world.
 * @param e The entity.
 * @return True if the entity is alive, false if the entity is not alive.
 */
FLECS_API
bool ecs_is_alive(
    const ecs_world_t *world,
    ecs_entity_t e);

/** Remove generation from entity id.
 *
 * @param e The entity id.
 * @return The entity id without the generation count.
 */
FLECS_API
ecs_id_t ecs_strip_generation(
    ecs_entity_t e);

/** Override the generation of an entity.
 * The generation count of an entity is increased each time an entity is deleted
 * and is used to test whether an entity id is alive.
 * 
 * This operation overrides the current generation of an entity with the
 * specified generation, which can be useful if an entity is externally managed,
 * like for external pools, savefiles or netcode.
 *
 * @param world The world.
 * @param entity Entity for which to set the generation with the new generation.
 */
FLECS_API
void ecs_set_entity_generation(
    ecs_world_t *world,
    ecs_entity_t entity);

/** Get alive identifier.
 * In some cases an application may need to work with identifiers from which
 * the generation has been stripped. A typical scenario in which this happens is
 * when iterating relationships in an entity type.
 *
 * For example, when obtaining the parent id from a ChildOf relationship, the parent
 * (second element of the pair) will have been stored in a 32 bit value, which 
 * cannot store the entity generation. This function can retrieve the identifier
 * with the current generation for that id.
 *
 * If the provided identifier is not alive, the function will return 0.
 *
 * @param world The world.
 * @param e The for which to obtain the current alive entity id.
 * @return The alive entity id if there is one, or 0 if the id is not alive.
 */
FLECS_API
ecs_entity_t ecs_get_alive(
    const ecs_world_t *world,
    ecs_entity_t e);

/** Ensure id is alive.
 * This operation ensures that the provided id is alive. This is useful in
 * scenarios where an application has an existing id that has not been created
 * with ecs_new (such as a global constant or an id from a remote application).
 * 
 * When this operation is successful it guarantees that the provided id exists, 
 * is valid and is alive.
 *
 * Before this operation the id must either not be alive or have a generation
 * that is equal to the passed in entity.
 *
 * If the provided id has a non-zero generation count and the id does not exist
 * in the world, the id will be created with the specified generation.
 * 
 * If the provided id is alive and has a generation count that does not match
 * the provided id, the operation will fail.
 *
 * @param world The world.
 * @param entity The entity id to make alive.
 */
FLECS_API
void ecs_ensure(
    ecs_world_t *world,
    ecs_entity_t entity);

/** Same as ecs_ensure, but for (component) ids.
 * An id can be an entity or pair, and can contain id flags. This operation
 * ensures that the entity (or entities, for a pair) are alive.
 * 
 * When this operation is successful it guarantees that the provided id can be
 * used in operations that accept an id.
 * 
 * Since entities in a pair do not encode their generation ids, this operation
 * will not fail when an entity with non-zero generation count already exists in
 * the world. 
 * 
 * This is different from ecs_ensure, which will fail if attempted with an id
 * that has generation 0 and an entity with a non-zero generation is currently 
 * alive.
 * 
 * @param world The world.
 * @param id The id to make alive.
 */
FLECS_API
void ecs_ensure_id(
    ecs_world_t *world,
    ecs_id_t id);

/** Test whether an entity exists.
 * Similar as ecs_is_alive, but ignores entity generation count.
 *
 * @param world The world.
 * @param entity The entity.
 * @return True if the entity exists, false if the entity does not exist.
 */
FLECS_API
bool ecs_exists(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** @} */

/**
 * @defgroup entity_info Entity Information.
 * @brief Get information from entity.
 * @{
 */

/** Get the type of an entity.
 *
 * @param world The world.
 * @param entity The entity.
 * @return The type of the entity, NULL if the entity has no components.
 */
FLECS_API
const ecs_type_t* ecs_get_type(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get the table of an entity.
 *
 * @param world The world.
 * @param entity The entity.
 * @return The table of the entity, NULL if the entity has no components/tags.
 */
FLECS_API
ecs_table_t* ecs_get_table(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Convert type to string.
 * The result of this operation must be freed with ecs_os_free.
 * 
 * @param world The world.
 * @param type The type.
 * @return The stringified type.
 */
FLECS_API
char* ecs_type_str(
    const ecs_world_t *world,
    const ecs_type_t* type);

/** Convert table to string.
 * Same as ecs_type_str(world, ecs_table_get_type(table)). The result of this
 * operation must be freed with ecs_os_free.
 *
 * @param world The world.
 * @param table The table.
 * @return The stringified table type.
 */
FLECS_API
char* ecs_table_str(
    const ecs_world_t *world,
    const ecs_table_t *table);

/** Convert entity to string.
 * Same as combining:
 * - ecs_get_fullpath(world, entity)
 * - ecs_type_str(world, ecs_get_type(world, entity))
 * 
 * The result of this operation must be freed with ecs_os_free.
 *
 * @param world The world.
 * @param entity The entity.
 * @return The entity path with stringified type.
 */
FLECS_API
char* ecs_entity_str(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Test if an entity has an id.
 * This operation returns true if the entity has or inherits the specified id.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id to test for.
 * @return True if the entity has the id, false if not.
 */
FLECS_API
bool ecs_has_id(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Test if an entity owns an id.
 * This operation returns true if the entity has the specified id. The operation
 * behaves the same as ecs_has_id, except that it will return false for 
 * components that are inherited through an IsA relationship.
 *
 * @param world The world.
 * @param entity The entity.
 * @param id The id to test for.
 * @return True if the entity has the id, false if not.
 */
FLECS_API
bool ecs_owns_id(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_id_t id);

/** Get the target of a relationship.
 * This will return a target (second element of a pair) of the entity for the 
 * specified relationship. The index allows for iterating through the targets, 
 * if a single entity has multiple targets for the same relationship.
 *
 * If the index is larger than the total number of instances the entity has for
 * the relationship, the operation will return 0.
 *
 * @param world The world.
 * @param entity The entity.
 * @param rel The relationship between the entity and the target.
 * @param index The index of the relationship instance.
 * @return The target for the relationship at the specified index.
 */
FLECS_API
ecs_entity_t ecs_get_target(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_entity_t rel,
    int32_t index);

/** Get parent (target of ChildOf relationship) for entity.
 * This operation is the same as calling:
 *   ecs_get_target(world, entity, EcsChildOf, 0);
 * 
 * @param world The world.
 * @param entity The entity.
 * @return The parent of the entity, 0 if the entity has no parent.
 */
FLECS_API
ecs_entity_t ecs_get_parent(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get the target of a relationship for a given id.
 * This operation returns the first entity that has the provided id by following
 * the specified relationship. If the entity itself has the id then entity will
 * be returned. If the id cannot be found on the entity or by following the
 * relationship, the operation will return 0.
 * 
 * This operation can be used to lookup, for example, which prefab is providing
 * a component by specifying the IsA relationship:
 * 
 *   // Is Position provided by the entity or one of its base entities?
 *   ecs_get_target_for_id(world, entity, EcsIsA, ecs_id(Position))
 * 
 * @param world The world.
 * @param entity The entity.
 * @param rel The relationship to follow.
 * @param id The id to lookup.
 * @return The entity for which the target has been found.
 */
FLECS_API
ecs_entity_t ecs_get_target_for_id(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_entity_t rel,
    ecs_id_t id);

/** Return depth for entity in tree for the specified relationship.
 * Depth is determined by counting the number of targets encountered while 
 * traversing up the relationship tree for rel. Only acyclic relationships are
 * supported.
 * 
 * @param world The world.
 * @param entity The entity.
 * @param rel The relationship.
 * @return The depth of the entity in the tree.
 */
FLECS_API
int32_t ecs_get_depth(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_entity_t rel);

typedef struct ecs_flatten_desc_t {
    /* When true, the flatten operation will not remove names from entities in
     * the flattened tree. This may fail if entities from different subtrees
     * have the same name. */
    bool keep_names;

    /* When true, the flattened tree won't contain information about the 
     * original depth of the entities. This can reduce fragmentation, but may
     * cause existing code, such as cascade queries, to no longer work. */
    bool lose_depth;
} ecs_flatten_desc_t;

/** Recursively flatten relationship for target entity (experimental).
 * This operation combines entities in the subtree of the specified pair from
 * different parents in the same table. This can reduce memory fragmentation
 * and reduces the number of tables in the storage, which improves RAM 
 * utilization and various other operations, such as entity cleanup.
 * 
 * The lifecycle of entities in a fixed subtree are bound to the specified
 * parent. Entities in a fixed subtree cannot be deleted individually. Entities
 * can also not change the target of the fixed relationship, which includes
 * removing the relationship.
 * 
 * Entities in a fixed subtree are still fragmented on subtree depth. This 
 * ensures that entities can still be iterated in breadth-first order with the
 * cascade query modifier.
 * 
 * The current implementation is limited to exclusive acyclic relationships, and
 * does not allow for adding/removing to entities in flattened tables. An entity
 * may only be flattened for a single relationship. Future iterations of the
 * feature may remove these limitations.
 * 
 * @param world The world.
 * @param pair The relationship pair from which to start flattening.
 * @param desc Options for flattening the tree.
 */
FLECS_API
void ecs_flatten(
    ecs_world_t *world,
    ecs_id_t pair,
    const ecs_flatten_desc_t *desc);

/** Count entities that have the specified id.
 * Returns the number of entities that have the specified id.
 *
 * @param world The world.
 * @param entity The id to search for.
 * @return The number of entities that have the id.
 */
FLECS_API
int32_t ecs_count_id(
    const ecs_world_t *world,
    ecs_id_t entity);

/** @} */


/**
 * @defgroup paths Entity Names
 * @brief Functions for working with entity names and paths.
 * @{
 */

/** Get the name of an entity.
 * This will return the name stored in (EcsIdentifier, EcsName).
 *
 * @param world The world.
 * @param entity The entity.
 * @return The type of the entity, NULL if the entity has no name.
 */
FLECS_API
const char* ecs_get_name(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get the symbol of an entity.
 * This will return the symbol stored in (EcsIdentifier, EcsSymbol).
 *
 * @param world The world.
 * @param entity The entity.
 * @return The type of the entity, NULL if the entity has no name.
 */
FLECS_API
const char* ecs_get_symbol(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Set the name of an entity.
 * This will set or overwrite the name of an entity. If no entity is provided,
 * a new entity will be created.
 *
 * The name is stored in (EcsIdentifier, EcsName).
 *
 * @param world The world.
 * @param entity The entity.
 * @param name The name.
 * @return The provided entity, or a new entity if 0 was provided.
 */
FLECS_API
ecs_entity_t ecs_set_name(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *name);

/** Set the symbol of an entity.
 * This will set or overwrite the symbol of an entity. If no entity is provided,
 * a new entity will be created.
 *
 * The symbol is stored in (EcsIdentifier, EcsSymbol).
 *
 * @param world The world.
 * @param entity The entity.
 * @param symbol The symbol.
 * @return The provided entity, or a new entity if 0 was provided.
 */
FLECS_API
ecs_entity_t ecs_set_symbol(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *symbol);

/** Set alias for entity. 
 * An entity can be looked up using its alias from the root scope without 
 * providing the fully qualified name if its parent. An entity can only have
 * a single alias.
 * 
 * The symbol is stored in (EcsIdentifier, EcsAlias).
 * 
 * @param world The world.
 * @param entity The entity.
 * @param alias The alias.
 */
FLECS_API
void ecs_set_alias(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *alias);

/** Lookup an entity by name.
 * Returns an entity that matches the specified name. Only looks for entities in
 * the current scope (root if no scope is provided).
 *
 * @param world The world.
 * @param name The entity name.
 * @return The entity with the specified name, or 0 if no entity was found.
 */
FLECS_API
ecs_entity_t ecs_lookup(
    const ecs_world_t *world,
    const char *name);

/** Lookup a child entity by name.
 * Returns an entity that matches the specified name. Only looks for entities in
 * the provided parent. If no parent is provided, look in the current scope (
 * root if no scope is provided).
 *
 * @param world The world.
 * @param name The entity name.
 * @return The entity with the specified name, or 0 if no entity was found.
 */
FLECS_API
ecs_entity_t ecs_lookup_child(
    const ecs_world_t *world,
    ecs_entity_t parent,
    const char *name);

/** Lookup an entity from a path.
 * Lookup an entity from a provided path, relative to the provided parent. The
 * operation will use the provided separator to tokenize the path expression. If
 * the provided path contains the prefix, the search will start from the root.
 *
 * If the entity is not found in the provided parent, the operation will 
 * continue to search in the parent of the parent, until the root is reached. If
 * the entity is still not found, the lookup will search in the flecs.core
 * scope. If the entity is not found there either, the function returns 0.
 *
 * @param world The world.
 * @param parent The entity from which to resolve the path.
 * @param path The path to resolve.
 * @param sep The path separator.
 * @param prefix The path prefix.
 * @param recursive Recursively traverse up the tree until entity is found.
 * @return The entity if found, else 0.
 */
FLECS_API
ecs_entity_t ecs_lookup_path_w_sep(
    const ecs_world_t *world,
    ecs_entity_t parent,
    const char *path,
    const char *sep,
    const char *prefix,
    bool recursive);

/** Lookup an entity by its symbol name.
 * This looks up an entity by symbol stored in (EcsIdentifier, EcsSymbol). The
 * operation does not take into account hierarchies.
 *
 * This operation can be useful to resolve, for example, a type by its C 
 * identifier, which does not include the Flecs namespacing.
 * 
 * @param world The world.
 * @param symbol The symbol.
 * @param lookup_as_path If not found as a symbol, lookup as path.
 * @param recursive If looking up as path, recursively traverse up the tree.
 * @return The entity if found, else 0.
 */
FLECS_API
ecs_entity_t ecs_lookup_symbol(
    const ecs_world_t *world,
    const char *symbol,
    bool lookup_as_path,
    bool recursive);

/** Get a path identifier for an entity.
 * This operation creates a path that contains the names of the entities from
 * the specified parent to the provided entity, separated by the provided 
 * separator. If no parent is provided the path will be relative to the root. If
 * a prefix is provided, the path will be prefixed by the prefix.
 *
 * If the parent is equal to the provided child, the operation will return an
 * empty string. If a nonzero component is provided, the path will be created by 
 * looking for parents with that component.
 *
 * The returned path should be freed by the application.
 *
 * @param world The world.
 * @param parent The entity from which to create the path.
 * @param child The entity to which to create the path.
 * @param sep The separator to use between path elements.
 * @param prefix The initial character to use for root elements.
 * @return The relative entity path.
 */
FLECS_API
char* ecs_get_path_w_sep(
    const ecs_world_t *world,
    ecs_entity_t parent,
    ecs_entity_t child,
    const char *sep,
    const char *prefix);

/** Write path identifier to buffer.
 * Same as ecs_get_path_w_sep, but writes result to an ecs_strbuf_t.
 * 
 * @param world The world.
 * @param parent The entity from which to create the path.
 * @param child The entity to which to create the path.
 * @param sep The separator to use between path elements.
 * @param prefix The initial character to use for root elements.
 * @param buf The buffer to write to.
 */
void ecs_get_path_w_sep_buf(
    const ecs_world_t *world,
    ecs_entity_t parent,
    ecs_entity_t child,
    const char *sep,
    const char *prefix,
    ecs_strbuf_t *buf);

/** Find or create entity from path.
 * This operation will find or create an entity from a path, and will create any
 * intermediate entities if required. If the entity already exists, no entities
 * will be created.
 *
 * If the path starts with the prefix, then the entity will be created from the
 * root scope.
 *
 * @param world The world.
 * @param parent The entity relative to which the entity should be created.
 * @param path The path to create the entity for.
 * @param sep The separator used in the path.
 * @param prefix The prefix used in the path.
 * @return The entity.
 */
FLECS_API
ecs_entity_t ecs_new_from_path_w_sep(
    ecs_world_t *world,
    ecs_entity_t parent,
    const char *path,
    const char *sep,
    const char *prefix);

/** Add specified path to entity.
 * This operation is similar to ecs_new_from_path, but will instead add the path
 * to an existing entity.
 *
 * If an entity already exists for the path, it will be returned instead.
 *
 * @param world The world.
 * @param entity The entity to which to add the path.
 * @param parent The entity relative to which the entity should be created.
 * @param path The path to create the entity for.
 * @param sep The separator used in the path.
 * @param prefix The prefix used in the path.
 * @return The entity.
 */ 
FLECS_API
ecs_entity_t ecs_add_path_w_sep(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_entity_t parent,
    const char *path,
    const char *sep,
    const char *prefix);

/** Set the current scope.
 * This operation sets the scope of the current stage to the provided entity.
 * As a result new entities will be created in this scope, and lookups will be
 * relative to the provided scope.
 *
 * It is considered good practice to restore the scope to the old value.
 *
 * @param world The world.
 * @param scope The entity to use as scope.
 * @return The previous scope.
 */
FLECS_API
ecs_entity_t ecs_set_scope(
    ecs_world_t *world,
    ecs_entity_t scope);

/** Get the current scope.
 * Get the scope set by ecs_set_scope. If no scope is set, this operation will
 * return 0.
 *
 * @param world The world.
 * @return The current scope.
 */
FLECS_API
ecs_entity_t ecs_get_scope(
    const ecs_world_t *world);

/** Set a name prefix for newly created entities.
 * This is a utility that lets C modules use prefixed names for C types and
 * C functions, while using names for the entity names that do not have the 
 * prefix. The name prefix is currently only used by ECS_COMPONENT.
 *
 * @param world The world.
 * @param prefix The name prefix to use.
 * @return The previous prefix.
 */
FLECS_API
const char* ecs_set_name_prefix(
    ecs_world_t *world,
    const char *prefix);    

/** Set search path for lookup operations.
 * This operation accepts an array of entity ids that will be used as search
 * scopes by lookup operations. The operation returns the current search path.
 * It is good practice to restore the old search path.
 * 
 * The search path will be evaluated starting from the last element.
 * 
 * The default search path includes flecs.core. When a custom search path is
 * provided it overwrites the existing search path. Operations that rely on
 * looking up names from flecs.core without providing the namespace may fail if
 * the custom search path does not include flecs.core (EcsFlecsCore).
 * 
 * The search path array is not copied into managed memory. The application must
 * ensure that the provided array is valid for as long as it is used as the
 * search path.
 * 
 * The provided array must be terminated with a 0 element. This enables an
 * application to push/pop elements to an existing array without invoking the
 * ecs_set_lookup_path operation again.
 * 
 * @param world The world.
 * @param lookup_path 0-terminated array with entity ids for the lookup path.
 * @return Current lookup path array.
 */
FLECS_API
ecs_entity_t* ecs_set_lookup_path(
    ecs_world_t *world,
    const ecs_entity_t *lookup_path);

/** Get current lookup path.
 * Returns value set by ecs_set_lookup_path.
 * 
 * @param world The world.
 * @return The current lookup path.
 */
FLECS_API
ecs_entity_t* ecs_get_lookup_path(
    const ecs_world_t *world);

/** @} */

/** @} */

/**
 * @defgroup components Components
 * @brief Functions for registering and working with components.
 * @{
 */

/** Find or create a component. 
 * This operation creates a new component, or finds an existing one. The find or
 * create behavior is the same as ecs_entity_init.
 *
 * When an existing component is found, the size and alignment are verified with
 * the provided values. If the values do not match, the operation will fail.
 *
 * See the documentation of ecs_component_desc_t for more details. 
 *
 * @param world The world.
 * @param desc Component init parameters.
 * @return A handle to the new or existing component, or 0 if failed.
 */
FLECS_API
ecs_entity_t ecs_component_init(
    ecs_world_t *world,
    const ecs_component_desc_t *desc); 

/** Get the type for an id.
 * This function returnsthe type information for an id. The specified id can be
 * any valid id. For the rules on how type information is determined based on
 * id, see ecs_get_typeid.
 * 
 * @param world The world.
 * @param id The id.
 * @return The type information of the id.
 */
FLECS_API
const ecs_type_info_t* ecs_get_type_info(
    const ecs_world_t *world,
    ecs_id_t id);

/** Register hooks for component.
 * Hooks allow for the execution of user code when components are constructed,
 * copied, moved, destructed, added, removed or set. Hooks can be assigned as
 * as long as a component has not yet been used (added to an entity).
 * 
 * The hooks that are currently set can be accessed with ecs_get_type_info.
 *
 * @param world The world.
 * @param id The component id for which to register the actions
 * @param hooks Type that contains the component actions.
 */
FLECS_API
void ecs_set_hooks_id(
    ecs_world_t *world,
    ecs_entity_t id,
    const ecs_type_hooks_t *hooks);

/** Get hooks for component.
 * 
 * @param world The world.
 * @param id The component id for which to retrieve the hooks.
 * @return The hooks for the component, or NULL if not registered.
 */
FLECS_API
const ecs_type_hooks_t* ecs_get_hooks_id(
    ecs_world_t *world,
    ecs_entity_t id);

/** @} */

/**
 * @defgroup ids Ids
 * @brief Functions for working with `ecs_id_t`.
 * @{
 */

/** Returns whether specified id a tag.
 * This operation returns whether the specified type is a tag (a component 
 * without data/size).
 * 
 * An id is a tag when:
 * - it is an entity without the EcsComponent component
 * - it has an EcsComponent with size member set to 0
 * - it is a pair where both elements are a tag
 * - it is a pair where the first element has the EcsTag tag
 * 
 * @param world The world.
 * @param id The id.
 * @return Whether the provided id is a tag.
 */
FLECS_API
bool ecs_id_is_tag(
    const ecs_world_t *world,
    ecs_id_t id);

/** Return whether represents a union.
 * This operation returns whether the specified type represents a union. Only
 * pair ids can be unions.
 * 
 * An id represents a union when:
 * - The first element of the pair is EcsUnion/flecs::Union
 * - The first element of the pair has EcsUnion/flecs::Union
 *
 * @param world The world.
 * @param id The id.
 * @return Whether the provided id represents a union.
 */
FLECS_API
bool ecs_id_is_union(
    const ecs_world_t *world,
    ecs_id_t id);

/** Returns whether specified id is in use.
 * This operation returns whether an id is in use in the world. An id is in use
 * if it has been added to one or more tables.
 * 
 * @param world The world.
 * @param id The id.
 * @return Whether the id is in use.
 */
FLECS_API
bool ecs_id_in_use(
    const ecs_world_t *world,
    ecs_id_t id);

/** Get the type for an id.
 * This operation returns the component id for an id, if the id is associated
 * with a type. For a regular component with a non-zero size (an entity with the
 * EcsComponent component) the operation will return the entity itself.
 * 
 * For an entity that does not have the EcsComponent component, or with an
 * EcsComponent value with size 0, the operation will return 0.
 * 
 * For a pair id the operation will return the type associated with the pair, by
 * applying the following rules in order:
 * - The first pair element is returned if it is a component
 * - 0 is returned if the relationship entity has the Tag property
 * - The second pair element is returned if it is a component
 * - 0 is returned.
 *
 * @param world The world.
 * @param id The id.
 * @return The type id of the id.
 */
FLECS_API
ecs_entity_t ecs_get_typeid(
    const ecs_world_t *world,
    ecs_id_t id);

/** Utility to match an id with a pattern.
 * This operation returns true if the provided pattern matches the provided
 * id. The pattern may contain a wildcard (or wildcards, when a pair).
 *
 * @param id The id.
 * @param pattern The pattern to compare with.
 */
FLECS_API
bool ecs_id_match(
    ecs_id_t id,
    ecs_id_t pattern);

/** Utility to check if id is a pair.
 *
 * @param id The id.
 * @return True if id is a pair.
 */
FLECS_API
bool ecs_id_is_pair(
    ecs_id_t id);

/** Utility to check if id is a wildcard.
 *
 * @param id The id.
 * @return True if id is a wildcard or a pair containing a wildcard.
 */
FLECS_API
bool ecs_id_is_wildcard(
    ecs_id_t id);

/** Utility to check if id is valid.
 * A valid id is an id that can be added to an entity. Invalid ids are:
 * - ids that contain wildcards
 * - ids that contain invalid entities
 * - ids that are 0 or contain 0 entities
 *
 * Note that the same rules apply to removing from an entity, with the exception
 * of wildcards.
 *
 * @param world The world.
 * @param id The id.
 * @return True if the id is valid.
 */
FLECS_API
bool ecs_id_is_valid(
    const ecs_world_t *world,
    ecs_id_t id);

/** Get flags associated with id.
 * This operation returns the internal flags (see api_flags.h) that are 
 * associated with the provided id.
 * 
 * @param world The world.
 * @param id The id.
 * @return Flags associated with the id, or 0 if the id is not in use.
 */
FLECS_API
ecs_flags32_t ecs_id_get_flags(
    const ecs_world_t *world,
    ecs_id_t id);

/** Convert id flag to string.
 * This operation converts a id flag to a string.
 * 
 * @param id_flags The id flag.
 * @return The id flag string, or NULL if no valid id is provided.
 */
FLECS_API
const char* ecs_id_flag_str(
    ecs_id_t id_flags);

/** Convert id to string.
 * This operation interprets the structure of an id and converts it to a string.
 *
 * @param world The world.
 * @param id The id to convert to a string.
 * @return The id converted to a string.
 */
FLECS_API
char* ecs_id_str(
    const ecs_world_t *world,
    ecs_id_t id);

/** Write id string to buffer.
 * Same as ecs_id_str but writes result to ecs_strbuf_t.
 *
 * @param world The world.
 * @param id The id to convert to a string.
 * @param buf The buffer to write to.
 */
FLECS_API
void ecs_id_str_buf(
    const ecs_world_t *world,
    ecs_id_t id,
    ecs_strbuf_t *buf);

/** @} */

/**
 * @defgroup filters Filters
 * @brief Functions for working with `ecs_term_t` and `ecs_filter_t`.
 * @{
 */

/** Iterator for a single (component) id.
 * A term iterator returns all entities (tables) that match a single (component)
 * id. The search for the matching set of entities (tables) is performed in 
 * constant time.
 *
 * @param world The world.
 * @param term The term.
 * @return The iterator.
 */
FLECS_API
ecs_iter_t ecs_term_iter(
    const ecs_world_t *world,
    ecs_term_t *term);

/** Return a chained term iterator.
 * A chained iterator applies a filter to the results of the input iterator. The
 * resulting iterator must be iterated with ecs_term_next.
 * 
 * @param it The input iterator
 * @param term The term filter to apply to the iterator.
 * @return The chained iterator. 
 */
FLECS_API
ecs_iter_t ecs_term_chain_iter(
    const ecs_iter_t *it,
    ecs_term_t *term);

/** Progress a term iterator.
 * This operation progresses the term iterator to the next table. The 
 * iterator must have been initialized with `ecs_term_iter`. This operation 
 * must be invoked at least once before interpreting the contents of the 
 * iterator.
 *
 * @param it The iterator.
 * @returns True if more data is available, false if not.
 */
FLECS_API
bool ecs_term_next(
    ecs_iter_t *it);

/** Iterator for a parent's children.
 * This operation is equivalent to a term iterator for (ChildOf, parent). 
 * Iterate the result with ecs_children_next.
 * 
 * @param world The world.
 * @param parent The parent for which to iterate the children.
 * @return The iterator.
 */
FLECS_API
ecs_iter_t ecs_children(
    const ecs_world_t *world,
    ecs_entity_t parent);

/** Progress a children iterator.
 * Equivalent to ecs_term_next.
 * 
 * @param it The iterator.
 * @returns True if more data is available, false if not.
 */
FLECS_API
bool ecs_children_next(
    ecs_iter_t *it);

/** Test whether term id is set.
 *
 * @param id The term id.
 * @return True when set, false when not set.
 */
FLECS_API 
bool ecs_term_id_is_set(
    const ecs_term_id_t *id);

/** Test whether a term is set.
 * This operation can be used to test whether a term has been initialized with
 * values or whether it is empty.
 *
 * An application generally does not need to invoke this operation. It is useful
 * when initializing a 0-initialized array of terms (like in ecs_term_desc_t) as
 * this operation can be used to find the last initialized element.
 *
 * @param term The term.
 * @return True when set, false when not set.
 */
FLECS_API
bool ecs_term_is_initialized(
    const ecs_term_t *term);

/** Is term matched on $this variable.
 * This operation checks whether a term is matched on the $this variable, which
 * is the default source for queries.
 * 
 * A term has a $this source when:
 * - ecs_term_t::src::id is EcsThis
 * - ecs_term_t::src::flags is EcsIsVariable
 * 
 * If ecs_term_t::src is not populated, it will be automatically initialized to
 * the $this source for the created query.
 * 
 * @param term The term.
 * @return True if term matches $this, false if not.
 */
FLECS_API
bool ecs_term_match_this(
    const ecs_term_t *term);

/** Is term matched on 0 source.
 * This operation checks whether a term is matched on a 0 source. A 0 source is
 * a term that isn't matched against anything, and can be used just to pass 
 * (component) ids to a query iterator.
 * 
 * A term has a 0 source when:
 * - ecs_term_t::src::id is 0
 * - ecs_term_t::src::flags has EcsIsEntity set
 * 
 * @param term The term.
 * @return True if term has 0 source, false if not.
 */
FLECS_API
bool ecs_term_match_0(
    const ecs_term_t *term);

/** Finalize term.
 * Ensure that all fields of a term are consistent and filled out. This 
 * operation should be invoked before using and after assigning members to, or 
 * parsing a term. When a term contains unresolved identifiers, this operation
 * will resolve and assign the identifiers. If the term contains any identifiers
 * that cannot be resolved, the operation will fail.
 *
 * An application generally does not need to invoke this operation as the APIs
 * that use terms (such as filters, queries and triggers) will finalize terms
 * when they are created.
 *
 * The name and expr parameters are optional, and only used for giving more 
 * descriptive error messages.
 *
 * @param world The world.
 * @param term The term to finalize.
 * @return Zero if success, nonzero if an error occurred.
 */
FLECS_API 
int ecs_term_finalize(
    const ecs_world_t *world,
    ecs_term_t *term);

/** Copy resources of a term to another term.
 * This operation copies one term to another term. If the source term contains
 * allocated resources (such as identifiers), they will be duplicated so that
 * no memory is shared between the terms.
 *
 * @param src The term to copy from.
 * @return The destination term.
 */
FLECS_API 
ecs_term_t ecs_term_copy(
    const ecs_term_t *src);

/** Move resources of a term to another term.
 * Same as copy, but moves resources from src, if src->move is set to true. If
 * src->move is not set to true, this operation will do a copy.
 *
 * The conditional move reduces redundant allocations in scenarios where a list 
 * of terms is partially created with allocated resources.
 *
 * @param src The term to move from.
 * @return The destination term.
 */
FLECS_API 
ecs_term_t ecs_term_move(
    ecs_term_t *src);    

/** Free resources of term.
 * This operation frees all resources (such as identifiers) of a term. The term
 * itself is not freed.
 *
 * @param term The term to free.
 */
FLECS_API
void ecs_term_fini(
    ecs_term_t *term);

/** Initialize filter 
 * A filter is a lightweight object that can be used to query for entities in
 * a world. Filters, as opposed to queries, do not cache results. They are 
 * therefore slower to iterate, but are faster to create.
 * 
 * When a filter is copied by value, make sure to use "ecs_filter_move" to 
 * ensure that the terms pointer still points to the inline array:
 * 
 *   ecs_filter_move(&dst_filter, &src_filter)
 * 
 * Alternatively, the ecs_filter_move function can be called with both arguments
 * set to the same filter, to ensure the pointer is valid:
 * 
 *   ecs_filter_move(&f, &f)
 *
 * It is possible to create a filter without allocating any memory, by setting
 * the .storage member in ecs_filter_desc_t. See the documentation for the 
 * member for more details.
 *
 * @param world The world.
 * @param desc Properties for the filter to create.
 * @return The filter if successful, NULL if not successful.
 */
FLECS_API
ecs_filter_t * ecs_filter_init(
    ecs_world_t *world,
    const ecs_filter_desc_t *desc);

/** Deinitialize filter.
 * Free resources associated with filter.
 *
 * @param filter The filter to deinitialize.
 */
FLECS_API
void ecs_filter_fini(
    ecs_filter_t *filter); 

/** Finalize filter.
 * When manually assigning an array of terms to the filter struct (so not when
 * using ecs_filter_init), this operation should be used to ensure that all 
 * terms are assigned properly and all (derived) fields have been set.
 *
 * When ecs_filter_init is used to create the filter, this function should not
 * be called. The purpose of this operation is to support creation of filters
 * without allocating memory.
 *
 * @param filter The filter to finalize.
 * @return Zero if filter is valid, non-zero if it contains errors.
 * @
 */
FLECS_API 
int ecs_filter_finalize(
    const ecs_world_t *world,
    ecs_filter_t *filter); 

/** Find index for $this variable.
 * This operation looks up the index of the $this variable. This index can
 * be used in operations like ecs_iter_set_var and ecs_iter_get_var.
 * 
 * The operation will return -1 if the variable was not found. This happens when
 * a filter only has terms that are not matched on the $this variable, like a
 * filter that exclusively matches singleton components.
 * 
 * @param filter The rule.
 * @return The index of the $this variable.
 */
FLECS_API
int32_t ecs_filter_find_this_var(
    const ecs_filter_t *filter);

/** Convert term to string expression.
 * Convert term to a string expression. The resulting expression is equivalent
 * to the same term, with the exception of And & Or operators.
 * 
 * @param world The world.
 * @param term The term.
 * @return The term converted to a string.
 */
FLECS_API
char* ecs_term_str(
    const ecs_world_t *world,
    const ecs_term_t *term);

/** Convert filter to string expression.
 * Convert filter terms to a string expression. The resulting expression can be
 * parsed to create the same filter.
 * 
 * @param world The world.
 * @param filter The filter.
 * @return The filter converted to a string.
 */
FLECS_API 
char* ecs_filter_str(
    const ecs_world_t *world,
    const ecs_filter_t *filter); 

/** Return a filter iterator.
 * A filter iterator lets an application iterate over entities that match the
 * specified filter.
 * 
 * @param world The world.
 * @param filter The filter.
 * @return An iterator that can be used with ecs_filter_next.
 */
FLECS_API
ecs_iter_t ecs_filter_iter(
    const ecs_world_t *world,
    const ecs_filter_t *filter);  

/** Return a chained filter iterator.
 * A chained iterator applies a filter to the results of the input iterator. The
 * resulting iterator must be iterated with ecs_filter_next.
 * 
 * @param it The input iterator
 * @param filter The filter to apply to the iterator.
 * @return The chained iterator. 
 */
FLECS_API
ecs_iter_t ecs_filter_chain_iter(
    const ecs_iter_t *it,
    const ecs_filter_t *filter);

/** Get pivot term for filter.
 * The pivot term is the term that matches the smallest set of tables, and is
 * a good default starting point for a search.
 * 
 * The following conditions must be met for a term to be considered as pivot:
 * - It must have a This subject
 * - It must have the And operator
 * 
 * When a filter does not have any terms that match those conditions, it will
 * return -1.
 * 
 * If one or more terms in the filter have no matching tables the filter won't
 * yield any results. In this case the operation will return -2 which gives a
 * search function the option to early out.
 * 
 * @param world The world.
 * @param filter The filter.
 * @return Index of the pivot term (use with filter->terms)
 */
FLECS_API
int32_t ecs_filter_pivot_term(
    const ecs_world_t *world,
    const ecs_filter_t *filter);

/** Iterate tables matched by filter.
 * This operation progresses the filter iterator to the next table. The 
 * iterator must have been initialized with `ecs_filter_iter`. This operation 
 * must be invoked at least once before interpreting the contents of the 
 * iterator.
 *
 * @param it The iterator
 * @return True if more data is available, false if not.
 */
FLECS_API
bool ecs_filter_next(
    ecs_iter_t *it);

/** Same as ecs_filter_next, but always instanced.
 * See instanced property of ecs_filter_desc_t.
 * 
 * @param it The iterator
 * @return True if more data is available, false if not.
 */
FLECS_API
bool ecs_filter_next_instanced(
    ecs_iter_t *it);

/** Move resources of one filter to another. 
 * 
 * @param dst The destination filter.
 * @param src The source filter.
 */
FLECS_API
void ecs_filter_move(
    ecs_filter_t *dst,
    ecs_filter_t *src);

/** Copy resources of one filter to another. 
 * 
 * @param dst The destination filter.
 * @param src The source filter.
 */
FLECS_API
void ecs_filter_copy(
    ecs_filter_t *dst,
    const ecs_filter_t *src);

/** @} */

/**
 * @defgroup queries Queries
 * @brief Functions for working with `ecs_query_t`.
 * @{
 */

/** Create a query.
 * This operation creates a query. Queries are used to iterate over entities
 * that match a filter and are the fastest way to find and iterate over entities
 * and their components.
 * 
 * Queries should be created once, and reused multiple times. While iterating a
 * query is a cheap operation, creating and deleting a query is expensive. The
 * reason for this is that queries are "prematched", which means that a query
 * stores state about which entities (or rather, tables) match with the query.
 * Building up this state happens during query creation.
 *
 * Once a query is created, matching only happens when new tables are created.
 * In most applications this is an infrequent process, since it only occurs when
 * a new combination of components is introduced. While matching is expensive,
 * it is importent to note that matching does not happen on a per-entity basis,
 * but on a per-table basis. This means that the average time spent on matching
 * per frame should rapidly approach zero over the lifetime of an application.
 *
 * A query provides direct access to the component arrays. When an application
 * creates/deletes entities or adds/removes components, these arrays can shift
 * component values around, or may grow in size. This can cause unexpected or
 * undefined behavior to occur if these operations are performed while 
 * iterating. To prevent this from happening an application should either not
 * perform these operations while iterating, or use deferred operations (see
 * ecs_defer_begin and ecs_defer_end).
 *
 * Queries can be created and deleted dynamically. If a query was not deleted
 * (using ecs_query_fini) before the world is deleted, it will be deleted 
 * automatically.
 *
 * @param world The world.
 * @param desc A structure describing the query properties.
 * @return The new query.
 */
FLECS_API
ecs_query_t* ecs_query_init(
    ecs_world_t *world, 
    const ecs_query_desc_t *desc);

/** Destroy a query.
 * This operation destroys a query and its resources. If the query is used as
 * the parent of subqueries, those subqueries will be orphaned and must be
 * deinitialized as well.
 *
 * @param query The query.
 */
FLECS_API
void ecs_query_fini(
    ecs_query_t *query);

/** Get filter from a query.
 * This operation obtains a pointer to the internally constructed filter
 * of the query and can be used to introspect the query terms.
 *
 * @param query The query.
 * @return The filter.
 */
FLECS_API
const ecs_filter_t* ecs_query_get_filter(
    const ecs_query_t *query);

/** Return a query iterator.
 * A query iterator lets an application iterate over entities that match the
 * specified query. If a sorting function is specified, the query will check
 * whether a resort is required upon creating the iterator.
 *
 * Creating a query iterator is a cheap operation that does not allocate any
 * resources. An application does not need to deinitialize or free a query 
 * iterator before it goes out of scope.
 *
 * To iterate the iterator, an application should use ecs_query_next to progress
 * the iterator and test if it has data.
 *
 * Query iteration requires an outer and an inner loop. The outer loop uses
 * ecs_query_next to test if new tables are available. The inner loop iterates
 * the entities in the table, and is usually a for loop that uses iter.count to
 * loop through the entities and component arrays.
 *
 * The two loops are necessary because of how data is stored internally. 
 * Entities are grouped by the components they have, in tables. A single query 
 * can (and often does) match with multiple tables. Because each table has its
 * own set of arrays, an application has to reobtain pointers to those arrays
 * for each matching table.
 *
 * @param world The world or stage, when iterating in readonly mode.
 * @param query The query to iterate.
 * @return The query iterator.
 */
FLECS_API
ecs_iter_t ecs_query_iter(
    const ecs_world_t *world,
    ecs_query_t *query);

/** Progress the query iterator.
 * This operation progresses the query iterator to the next table. The 
 * iterator must have been initialized with `ecs_query_iter`. This operation 
 * must be invoked at least once before interpreting the contents of the 
 * iterator.
 *
 * @param iter The iterator.
 * @returns True if more data is available, false if not.
 */
FLECS_API
bool ecs_query_next(
    ecs_iter_t *iter);

/** Same as ecs_query_next, but always instanced.
 * See "instanced" property of ecs_filter_desc_t.
 * 
 * @param iter The iterator.
 * @returns True if more data is available, false if not.
 */
FLECS_API
bool ecs_query_next_instanced(
    ecs_iter_t *iter);

/** Fast alternative to ecs_query_next that only returns matched tables.
 * This operation only populates the ecs_iter_t::table field. To access the
 * matched components, call ecs_query_populate.
 * 
 * If this operation is used with a query that has inout/out terms, those terms
 * will not be marked dirty unless ecs_query_populate is called. 
 * 
 * @param iter The iterator.
 * @returns True if more data is available, false if not.
 */
FLECS_API
bool ecs_query_next_table(
    ecs_iter_t *iter);

/** Populate iterator fields.
 * This operation can be combined with ecs_query_next_table to populate the
 * iterator fields for the current table.
 * 
 * Populating fields conditionally can save time when a query uses change 
 * detection, and only needs iterator data when the table has changed. When this
 * operation is called, inout/out terms will be marked dirty.
 * 
 * In cases where inout/out terms are conditionally written and no changes
 * were made after calling ecs_query_populate, the ecs_query_skip function can
 * be called to prevent the matched table components from being marked dirty.
 * 
 * This operation does should not be used with queries that match disabled 
 * components, union relationships, or with queries that use order_by.
 * 
 * When the when_changed argument is set to true, the iterator data will only
 * populate when the data has changed, using query change detection.
 * 
 * @param iter The iterator.
 * @param when_changed Only populate data when result has changed.
 */
FLECS_API
int ecs_query_populate(
    ecs_iter_t *iter,
    bool when_changed);

/** Returns whether the query data changed since the last iteration.
 * The operation will return true after:
 * - new entities have been matched with
 * - new tables have been matched/unmatched with
 * - matched entities were deleted
 * - matched components were changed
 * 
 * The operation will not return true after a write-only (EcsOut) or filter
 * (EcsInOutNone) term has changed, when a term is not matched with the
 * current table (This subject) or for tag terms.
 * 
 * The changed state of a table is reset after it is iterated. If a iterator was
 * not iterated until completion, tables may still be marked as changed.
 * 
 * If no iterator is provided the operation will return the changed state of the
 * all matched tables of the query. 
 * 
 * If an iterator is provided, the operation will return the changed state of 
 * the currently returned iterator result. The following preconditions must be
 * met before using an iterator with change detection:
 * 
 * - The iterator is a query iterator (created with ecs_query_iter)
 * - The iterator must be valid (ecs_query_next must have returned true)
 * - The iterator must be instanced
 * 
 * @param query The query (optional if 'it' is provided).
 * @param it The iterator result to test (optional if 'query' is provided).
 * @return true if entities changed, otherwise false.
 */
FLECS_API
bool ecs_query_changed(
    ecs_query_t *query,
    const ecs_iter_t *it);

/** Skip a table while iterating.
 * This operation lets the query iterator know that a table was skipped while
 * iterating. A skipped table will not reset its changed state, and the query
 * will not update the dirty flags of the table for its out columns.
 * 
 * Only valid iterators must be provided (next has to be called at least once &
 * return true) and the iterator must be a query iterator.
 * 
 * @param it The iterator result to skip.
 */
FLECS_API
void ecs_query_skip(
    ecs_iter_t *it);

/** Set group to iterate for query iterator.
 * This operation limits the results returned by the query to only the selected
 * group id. The query must have a group_by function, and the iterator must
 * be a query iterator.
 * 
 * Groups are sets of tables that are stored together in the query cache based
 * on a group id, which is calculated per table by the group_by function. To 
 * iterate a group, an iterator only needs to know the first and last cache node
 * for that group, which can both be found in a fast O(1) operation.
 * 
 * As a result, group iteration is one of the most efficient mechanisms to 
 * filter out large numbers of entities, even if those entities are distributed
 * across many tables. This makes it a good fit for things like dividing up
 * a world into cells, and only iterating cells close to a player.
 * 
 * The group to iterate must be set before the first call to ecs_query_next. No
 * operations that can add/remove components should be invoked between calling 
 * ecs_query_set_group and ecs_query_next.
 * 
 * @param it The query iterator.
 * @param group_id The group to iterate.
 */
FLECS_API
void ecs_query_set_group(
    ecs_iter_t *it,
    uint64_t group_id);

/** Get context of query group.
 * This operation returns the context of a query group as returned by the 
 * on_group_create callback.
 * 
 * @param query The query.
 * @param group_id The group for which to obtain the context.
 * @return The group context, NULL if the group doesn't exist.
 */
FLECS_API
void* ecs_query_get_group_ctx(
    const ecs_query_t *query,
    uint64_t group_id);

/** Get information about query group.
 * This operation returns information about a query group, including the group
 * context returned by the on_group_create callback.
 * 
 * @param query The query.
 * @param group_id The group for which to obtain the group info.
 * @return The group info, NULL if the group doesn't exist.
 */
FLECS_API
const ecs_query_group_info_t* ecs_query_get_group_info(
    const ecs_query_t *query,
    uint64_t group_id);

/** Returns whether query is orphaned.
 * When the parent query of a subquery is deleted, it is left in an orphaned
 * state. The only valid operation on an orphaned query is deleting it. Only
 * subqueries can be orphaned.
 *
 * @param query The query.
 * @return true if query is orphaned, otherwise false.
 */
FLECS_API
bool ecs_query_orphaned(
    const ecs_query_t *query);

/** Convert query to string.
 *
 * @param query The query.
 * @return The query string.
 */
FLECS_API
char* ecs_query_str(
    const ecs_query_t *query);

/** Returns number of tables query matched with.
 *
 * @param query The query.
 * @return The number of matched tables.
 */
FLECS_API
int32_t ecs_query_table_count(
    const ecs_query_t *query);

/** Returns number of empty tables query matched with.
 *
 * @param query The query.
 * @return The number of matched empty tables.
 */
FLECS_API
int32_t ecs_query_empty_table_count(
    const ecs_query_t *query);

/** Returns number of entities query matched with.
 * This operation iterates all non-empty tables in the query cache to find the
 * total number of entities.
 *
 * @param query The query.
 * @return The number of matched entities.
 */
FLECS_API
int32_t ecs_query_entity_count(
    const ecs_query_t *query);

/** Get query ctx.
 * Return the value set in ecs_query_desc_t::ctx.
 * 
 * @param query The query.
 * @return The context.
 */
FLECS_API
void* ecs_query_get_ctx(
    const ecs_query_t *query);

/** Get query binding ctx.
 * Return the value set in ecs_query_desc_t::binding_ctx.
 * 
 * @param query The query.
 * @return The context.
 */
FLECS_API
void* ecs_query_get_binding_ctx(
    const ecs_query_t *query);

/** @} */

/**
 * @defgroup observer Observers
 * @brief Functions for working with events and observers.
 * @{
 */

/** Send event.
 * This sends an event to matching triggers & is the mechanism used by flecs
 * itself to send OnAdd, OnRemove, etc events.
 * 
 * Applications can use this function to send custom events, where a custom
 * event can be any regular entity.
 * 
 * Applications should not send builtin flecs events, as this may violate
 * assumptions the code makes about the conditions under which those events are
 * sent.
 * 
 * Triggers are invoked synchronously. It is therefore safe to use stack-based
 * data as event context, which can be set in the "param" member.
 * 
 * @param world The world.
 * @param desc Event parameters.
 */
FLECS_API
void ecs_emit( 
    ecs_world_t *world,
    ecs_event_desc_t *desc);

FLECS_API
void ecs_enqueue(
    ecs_world_t *world,
    ecs_event_desc_t *desc);

/** Create observer.
 * Observers are like triggers, but can subscribe for multiple terms. An 
 * observer only triggers when the source of the event meets all terms.
 *
 * See the documentation for ecs_observer_desc_t for more details.
 *
 * @param world The world.
 * @param desc The observer creation parameters.
 */
FLECS_API
ecs_entity_t ecs_observer_init(
    ecs_world_t *world,
    const ecs_observer_desc_t *desc);

/** Default run action for observer.
 * This function can be called from a custom observer run action (see 
 * ecs_observer_desc_t::run for more details). This function ensures that the 
 * observer's filter is applied to the iterator's table, filters out duplicate 
 * events and implements EcsMonitor logic.
 * 
 * @param it The iterator.
 * @return True if the observer was invoked.
 */
FLECS_API
bool ecs_observer_default_run_action(
    ecs_iter_t *it);

/** Get observer ctx. 
 * Return the value set in ecs_observer_desc_t::ctx.
 * 
 * @param world The world.
 * @param observer The observer.
 * @return The context.
 */
FLECS_API
void* ecs_observer_get_ctx(
    const ecs_world_t *world,
    ecs_entity_t observer);

/** Get observer binding ctx.
 * Return the value set in ecs_observer_desc_t::binding_ctx.
 * 
 * @param world The world.
 * @param observer The observer.
 * @return The context.
 */
FLECS_API
void* ecs_observer_get_binding_ctx(
    const ecs_world_t *world,
    ecs_entity_t observer);

/** @} */

/**
 * @defgroup iterator Iterators
 * @brief Functions for working with `ecs_iter_t`.
 * @{
 */

/** Create iterator from poly object.
 * The provided poly object must have the iterable mixin. If an object is 
 * provided that does not have the mixin, the function will assert. 
 * 
 * When a filter is provided, an array of two iterators must be passed to the 
 * function. This allows the mixin implementation to create a chained iterator 
 * when necessary, which requires two iterator objects.
 * 
 * If a filter is provided, the first element in the array of two iterators is
 * the one that should be iterated. The mixin implementation may or may not set
 * the second element, depending on whether an iterator chain is required.
 * 
 * Additionally, when a filter is provided the returned iterator will be for a
 * single term with the provided filter id. If the iterator is chained, the
 * previous iterator in the chain can be accessed through it->chain_it.
 * 
 * @param world The world or stage for which to create the iterator.
 * @param poly The poly object from which to create the iterator.
 * @param iter The iterator (out, ecs_iter_t[2] when filter is set).
 * @param filter Optional term used for filtering the results.
 */
FLECS_API
void ecs_iter_poly(
    const ecs_world_t *world,
    const ecs_poly_t *poly,
    ecs_iter_t *iter,
    ecs_term_t *filter);

/** Progress any iterator.
 * This operation is useful in combination with iterators for which it is not
 * known what created them. Example use cases are functions that should accept
 * any kind of iterator (such as serializers) or iterators created from poly
 * objects.
 * 
 * This operation is slightly slower than using a type-specific iterator (e.g.
 * ecs_filter_next, ecs_query_next) as it has to call a function pointer which
 * introduces a level of indirection.
 * 
 * @param it The iterator.
 * @return True if iterator has more results, false if not.
 */
FLECS_API
bool ecs_iter_next(
    ecs_iter_t *it);

/** Cleanup iterator resources.
 * This operation cleans up any resources associated with the iterator.
 * 
 * This operation should only be used when an iterator is not iterated until
 * completion (next has not yet returned false). When an iterator is iterated
 * until completion, resources are automatically freed.
 * 
 * @param it The iterator.
 */
FLECS_API
void ecs_iter_fini(
    ecs_iter_t *it);

/** Count number of matched entities in query.
 * This operation returns the number of matched entities. If a query contains no
 * matched entities but still yields results (e.g. it has no terms with This
 * sources) the operation will return 0.
 * 
 * To determine the number of matched entities, the operation iterates the
 * iterator until it yields no more results.
 * 
 * @param it The iterator.
 * @return True if iterator has more results, false if not.
 */
FLECS_API
int32_t ecs_iter_count(
    ecs_iter_t *it);

/** Test if iterator is true.
 * This operation will return true if the iterator returns at least one result.
 * This is especially useful in combination with fact-checking rules (see the
 * rules addon).
 * 
 * The operation requires a valid iterator. After the operation is invoked, the
 * application should no longer invoke next on the iterator and should treat it
 * as if the iterator is iterated until completion.
 * 
 * @param it The iterator.
 * @return true if the iterator returns at least one result.
 */
FLECS_API
bool ecs_iter_is_true(
    ecs_iter_t *it);

/** Get first matching entity from iterator.
 * After this operation the application should treat the iterator as if it has
 * been iterated until completion.
 * 
 * @param it The iterator.
 * @return The first matching entity, or 0 if no entities were matched.
 */
FLECS_API
ecs_entity_t ecs_iter_first(
    ecs_iter_t *it);

/** Set value for iterator variable.
 * This constrains the iterator to return only results for which the variable
 * equals the specified value. The default value for all variables is 
 * EcsWildcard, which means the variable can assume any value.
 * 
 * Example:
 * 
 * // Rule that matches (Eats, *)
 * ecs_rule_t *r = ecs_rule_init(world, &(ecs_filter_desc_t){
 *   .terms = {
 *     { .first.id = Eats, .second.name = "$food" }
 *   }
 * });
 * 
 * int food_var = ecs_rule_find_var(r, "food");
 * 
 * // Set Food to Apples, so we're only matching (Eats, Apples)
 * ecs_iter_t it = ecs_rule_iter(world, r);
 * ecs_iter_set_var(&it, food_var, Apples);
 * 
 * while (ecs_rule_next(&it)) {
 *   for (int i = 0; i < it.count; i ++) {
 *     // iterate as usual
 *   }
 * }
 * 
 * The variable must be initialized after creating the iterator and before the
 * first call to next.
 * 
 * @param it The iterator.
 * @param var_id The variable index.
 * @param entity The entity variable value.
 */
FLECS_API
void ecs_iter_set_var(
    ecs_iter_t *it,
    int32_t var_id,
    ecs_entity_t entity);

/** Same as ecs_iter_set_var, but for a table.
 * This constrains the variable to all entities in a table.
 * 
 * @param it The iterator.
 * @param var_id The variable index.
 * @param table The table variable value.
 */
FLECS_API
void ecs_iter_set_var_as_table(
    ecs_iter_t *it,
    int32_t var_id,
    const ecs_table_t *table);

/** Same as ecs_iter_set_var, but for a range of entities
 * This constrains the variable to a range of entities in a table.
 * 
 * @param it The iterator.
 * @param var_id The variable index.
 * @param range The range variable value.
 */
FLECS_API
void ecs_iter_set_var_as_range(
    ecs_iter_t *it,
    int32_t var_id,
    const ecs_table_range_t *range);

/** Get value of iterator variable as entity.
 * A variable can be interpreted as entity if it is set to an entity, or if it
 * is set to a table range with count 1.
 * 
 * This operation can only be invoked on valid iterators. The variable index
 * must be smaller than the total number of variables provided by the iterator
 * (as set in ecs_iter_t::variable_count).
 * 
 * @param it The iterator.
 * @param var_id The variable index.
 * @return The variable value.
 */
FLECS_API
ecs_entity_t ecs_iter_get_var(
    ecs_iter_t *it,
    int32_t var_id);

/** Get value of iterator variable as table.
 * A variable can be interpreted as table if it is set as table range with
 * both offset and count set to 0, or if offset is 0 and count matches the
 * number of elements in the table.
 * 
 * This operation can only be invoked on valid iterators. The variable index
 * must be smaller than the total number of variables provided by the iterator
 * (as set in ecs_iter_t::variable_count).
 * 
 * @param it The iterator.
 * @param var_id The variable index.
 * @return The variable value.
 */
FLECS_API
ecs_table_t* ecs_iter_get_var_as_table(
    ecs_iter_t *it,
    int32_t var_id);

/** Get value of iterator variable as table range.
 * A value can be interpreted as table range if it is set as table range, or if
 * it is set to an entity with a non-empty type (the entity must have at least
 * one component, tag or relationship in its type).
 * 
 * This operation can only be invoked on valid iterators. The variable index
 * must be smaller than the total number of variables provided by the iterator
 * (as set in ecs_iter_t::variable_count).
 * 
 * @param it The iterator.
 * @param var_id The variable index.
 * @return The variable value.
 */
FLECS_API
ecs_table_range_t ecs_iter_get_var_as_range(
    ecs_iter_t *it,
    int32_t var_id);

/** Returns whether variable is constrained.
 * This operation returns true for variables set by one of the ecs_iter_set_var*
 * operations.
 * 
 * A constrained variable is guaranteed not to change values while results are
 * being iterated.
 * 
 * @param it The iterator.
 * @param var_id The variable index.
 * @return Whether the variable is constrained to a specified value.
 */
FLECS_API
bool ecs_iter_var_is_constrained(
    ecs_iter_t *it,
    int32_t var_id);

/** Convert iterator to string.
 * Prints the contents of an iterator to a string. Useful for debugging and/or
 * testing the output of an iterator.
 * 
 * The function only converts the currently iterated data to a string. To 
 * convert all data, the application has to manually call the next function and
 * call ecs_iter_str on each result.
 * 
 * @param it The iterator.
 * @return A string representing the contents of the iterator.
 */
FLECS_API
char* ecs_iter_str(
    const ecs_iter_t *it);

/** Create a paged iterator.
 * Paged iterators limit the results to those starting from 'offset', and will
 * return at most 'limit' results.
 * 
 * The iterator must be iterated with ecs_page_next.
 * 
 * A paged iterator acts as a passthrough for data exposed by the parent
 * iterator, so that any data provided by the parent will also be provided by
 * the paged iterator.
 * 
 * @param it The source iterator.
 * @param offset The number of entities to skip.
 * @param limit The maximum number of entities to iterate.
 * @return A page iterator.
 */
FLECS_API
ecs_iter_t ecs_page_iter(
    const ecs_iter_t *it,
    int32_t offset,
    int32_t limit);

/** Progress a paged iterator.
 * Progresses an iterator created by ecs_page_iter.
 * 
 * @param it The iterator.
 * @return true if iterator has more results, false if not.
 */
FLECS_API
bool ecs_page_next(
    ecs_iter_t *it);

/** Create a worker iterator.
 * Worker iterators can be used to equally divide the number of matched entities 
 * across N resources (usually threads). Each resource will process the total
 * number of matched entities divided by 'count'.
 * 
 * Entities are distributed across resources such that the distribution is
 * stable between queries. Two queries that match the same table are guaranteed
 * to match the same entities in that table.
 * 
 * The iterator must be iterated with ecs_worker_next.
 * 
 * A worker iterator acts as a passthrough for data exposed by the parent
 * iterator, so that any data provided by the parent will also be provided by
 * the worker iterator.
 * 
 * @param it The source iterator.
 * @param index The index of the current resource.
 * @param count The total number of resources to divide entities between.
 * @return A worker iterator.
 */
FLECS_API
ecs_iter_t ecs_worker_iter(
    const ecs_iter_t *it,
    int32_t index,
    int32_t count);

/** Progress a worker iterator.
 * Progresses an iterator created by ecs_worker_iter.
 * 
 * @param it The iterator.
 * @return true if iterator has more results, false if not.
 */
FLECS_API
bool ecs_worker_next(
    ecs_iter_t *it);

/** Obtain data for a query field.
 * This operation retrieves a pointer to an array of data that belongs to the
 * term in the query. The index refers to the location of the term in the query,
 * and starts counting from one.
 *
 * For example, the query "Position, Velocity" will return the Position array
 * for index 1, and the Velocity array for index 2.
 *
 * When the specified field is not owned by the entity this function returns a
 * pointer instead of an array. This happens when the source of a field is not
 * the entity being iterated, such as a shared component (from a prefab), a
 * component from a parent, or another entity. The ecs_field_is_self operation
 * can be used to test dynamically if a field is owned.
 *
 * The provided size must be either 0 or must match the size of the datatype
 * of the returned array. If the size does not match, the operation may assert.
 * The size can be dynamically obtained with ecs_field_size.
 *
 * @param it The iterator.
 * @param size The type size of the requested data.
 * @param index The index of the field in the iterator.
 * @return A pointer to the data of the field.
 */
FLECS_API
void* ecs_field_w_size(
    const ecs_iter_t *it,
    size_t size,
    int32_t index);

/** Test whether the field is readonly.
 * This operation returns whether the field is readonly. Readonly fields are
 * annotated with [in], or are added as a const type in the C++ API.
 *
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return Whether the field is readonly.
 */
FLECS_API
bool ecs_field_is_readonly(
    const ecs_iter_t *it,
    int32_t index);    

/** Test whether the field is writeonly.
 * This operation returns whether this is a writeonly field. Writeonly terms are
 * annotated with [out].
 * 
 * Serializers are not required to serialize the values of a writeonly field.
 *
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return Whether the field is writeonly.
 */
FLECS_API
bool ecs_field_is_writeonly(
    const ecs_iter_t *it,
    int32_t index);

/** Test whether field is set.
 * 
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return Whether the field is set.
 */
FLECS_API
bool ecs_field_is_set(
    const ecs_iter_t *it,
    int32_t index); 

/** Return id matched for field.
 * 
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return The id matched for the field.
 */
FLECS_API
ecs_id_t ecs_field_id(
    const ecs_iter_t *it,
    int32_t index);

/** Return index of matched table column.
 * This function only returns column indices for fields that have been matched
 * on the $this variable. Fields matched on other tables will return -1.
 * 
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return The index of the matched column, -1 if not matched.
 */
FLECS_API
int32_t ecs_field_column_index(
    const ecs_iter_t *it,
    int32_t index);

/** Return field source.
 * The field source is the entity on which the field was matched.
 * 
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return The source for the field.
 */
FLECS_API
ecs_entity_t ecs_field_src(
    const ecs_iter_t *it,
    int32_t index);

/** Return field type size.
 * Return type size of the field. Returns 0 if the field has no data.
 * 
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return The type size for the field.
 */
FLECS_API
size_t ecs_field_size(
    const ecs_iter_t *it,
    int32_t index);

/** Test whether the field is matched on self.
 * This operation returns whether the field is matched on the currently iterated
 * entity. This function will return false when the field is owned by another
 * entity, such as a parent or a prefab.
 * 
 * When this operation returns false, the field must be accessed as a single 
 * value instead of an array. Fields for which this operation returns true
 * return arrays with it->count values.
 *
 * @param it The iterator.
 * @param index The index of the field in the iterator.
 * @return Whether the field is matched on self.
 */
FLECS_API
bool ecs_field_is_self(
    const ecs_iter_t *it,
    int32_t index);

/** @} */

/**
 * @defgroup tables Tables
 * @brief Functions for working with `ecs_table_t`.
 * @{
 */

/** Get type for table.
 * The table type is a vector that contains all component, tag and pair ids.
 *
 * @param table The table.
 * @return The type of the table.
 */
FLECS_API
const ecs_type_t* ecs_table_get_type(
    const ecs_table_t *table);

/** Get type index for id.
 * This operation returns the index for an id in the table's type.
 * 
 * @param world The world.
 * @param table The table.
 * @param id The id.
 * @return The index of the id in the table type, or -1 if not found.
 */
FLECS_API
int32_t ecs_table_get_type_index(
    const ecs_world_t *world,
    const ecs_table_t *table,
    ecs_id_t id);

/** Get column index for id.
 * This operation returns the column index for an id in the table's type. If the
 * id is not a component, the function will return -1.
 * 
 * @param world The world.
 * @param table The table.
 * @param id The component id.
 * @return The column index of the id, or -1 if not found/not a component.
 */
FLECS_API
int32_t ecs_table_get_column_index(
    const ecs_world_t *world,
    const ecs_table_t *table,
    ecs_id_t id);

/** Return number of columns in table. 
 * Similar to ecs_table_get_type(table)->count, except that the column count 
 * only counts the number of components in a table.
 * 
 * @param table The table.
 * @return The number of columns in the table.
 */
FLECS_API
int32_t ecs_table_column_count(
    const ecs_table_t *table);

/** Convert type index to column index. 
 * Tables have an array of columns for each component in the table. This array
 * does not include elements for tags, which means that the index for a 
 * component in the table type is not necessarily the same as the index in the
 * column array. This operation converts from an index in the table type to an
 * index in the column array.
 * 
 * @param table The table.
 * @param index The index in the table type.
 * @return The index in the table column array.
 */
FLECS_API
int32_t ecs_table_type_to_column_index(
    const ecs_table_t *table,
    int32_t index);

/** Convert column index to type index.
 * Same as ecs_table_type_to_column_index, but converts from an index in the
 * column array to an index in the table type.
 * 
 * @param table The table.
 * @param index The column index.
 * @return The index in the table type.
 */
FLECS_API
int32_t ecs_table_column_to_type_index(
    const ecs_table_t *table,
    int32_t index);

/** Get column from table by column index.
 * This operation returns the component array for the provided index.
 * 
 * @param table The table.
 * @param index The column index.
 * @param offset The index of the first row to return (0 for entire column).
 * @return The component array, or NULL if the index is not a component.
 */
FLECS_API
void* ecs_table_get_column(
    const ecs_table_t *table,
    int32_t index,
    int32_t offset);

/** Get column from table by component id.
 * This operation returns the component array for the provided component  id.
 * 
 * @param table The table.
 * @param id The component id for the column.
 * @param offset The index of the first row to return (0 for entire column).
 * @return The component array, or NULL if the index is not a component.
 */
FLECS_API
void* ecs_table_get_id(
    const ecs_world_t *world,
    const ecs_table_t *table,
    ecs_id_t id,
    int32_t offset);

/** Get column size from table.
 * This operation returns the component size for the provided index.
 * 
 * @param table The table.
 * @param index The column index.
 * @return The component size, or 0 if the index is not a component.
 */
FLECS_API
size_t ecs_table_get_column_size(
    const ecs_table_t *table,
    int32_t index);

/** Returns the number of records in the table. 
 * This operation returns the number of records that have been populated through
 * the regular (entity) API as well as the number of records that have been
 * inserted using the direct access API.
 *
 * @param table The table.
 * @return The number of records in a table.
 */
FLECS_API
int32_t ecs_table_count(
    const ecs_table_t *table);

/** Test if table has id.
 * Same as ecs_table_get_type_index(world, table, id) != -1.
 * 
 * @param world The world.
 * @param table The table.
 * @param id The id.
 * @return True if the table has the id, false if the table doesn't.
 */
FLECS_API
bool ecs_table_has_id(
    const ecs_world_t *world,
    const ecs_table_t *table,
    ecs_id_t id);

/** Return depth for table in tree for relationship rel.
 * Depth is determined by counting the number of targets encountered while 
 * traversing up the relationship tree for rel. Only acyclic relationships are
 * supported.
 * 
 * @param world The world.
 * @param table The table.
 * @param rel The relationship.
 * @return The depth of the table in the tree.
 */
FLECS_API
int32_t ecs_table_get_depth(
    const ecs_world_t *world,
    const ecs_table_t *table,
    ecs_entity_t rel);

/** Get table that has all components of current table plus the specified id.
 * If the provided table already has the provided id, the operation will return
 * the provided table.
 *
 * @param world The world.
 * @param table The table.
 * @param id The id to add.
 * @result The resulting table.
 */
FLECS_API
ecs_table_t* ecs_table_add_id(
    ecs_world_t *world,
    ecs_table_t *table,
    ecs_id_t id);

/** Find table from id array. 
 * This operation finds or creates a table with the specified array of 
 * (component) ids. The ids in the array must be sorted, and it may not contain
 * duplicate elements.
 * 
 * @param world The world.
 * @param ids The id array.
 * @param id_count The number of elements in the id array.
 * @return The table with the specified (component) ids.
 */
FLECS_API
ecs_table_t* ecs_table_find(
    ecs_world_t *world,
    const ecs_id_t *ids,
    int32_t id_count);

/** Get table that has all components of current table minus the specified id.
 * If the provided table doesn't have the provided id, the operation will return
 * the provided table.
 *
 * @param world The world.
 * @param table The table.
 * @param id The id to remove.
 * @result The resulting table.
 */
FLECS_API
ecs_table_t* ecs_table_remove_id(
    ecs_world_t *world,
    ecs_table_t *table,
    ecs_id_t id);

/** Lock or unlock table.
 * When a table is locked, modifications to it will throw an assert. When the 
 * table is locked recursively, it will take an equal amount of unlock
 * operations to actually unlock the table.
 *
 * Table locks can be used to build safe iterators where it is guaranteed that
 * the contents of a table are not modified while it is being iterated.
 *
 * The operation only works when called on the world, and has no side effects 
 * when called on a stage. The assumption is that when called on a stage,
 * operations are deferred already.
 *
 * @param world The world.
 * @param table The table to lock.
 */
FLECS_API
void ecs_table_lock(
    ecs_world_t *world,
    ecs_table_t *table);

/** Unlock a table.
 * Must be called after calling ecs_table_lock.
 *
 * @param world The world.
 * @param table The table to unlock.
 */
FLECS_API
void ecs_table_unlock(
    ecs_world_t *world,
    ecs_table_t *table);    

/** Test table for flags.
 * Test if table has all of the provided flags. See 
 * include/flecs/private/api_flags.h for a list of table flags that can be used 
 * with this function.
 *
 * @param table The table.
 * @param flags The flags to test for.
 * @return Whether the specified flags are set for the table.
 */
FLECS_API
bool ecs_table_has_flags(
    ecs_table_t *table,
    ecs_flags32_t flags);

/** Swaps two elements inside the table. This is useful for implementing custom
 * table sorting algorithms.
 * @param world The world
 * @param table The table to swap elements in
 * @param row_1 Table element to swap with row_2
 * @param row_2 Table element to swap with row_1
*/
FLECS_API
void ecs_table_swap_rows(
    ecs_world_t* world,
    ecs_table_t* table,
    int32_t row_1,
    int32_t row_2);

/** Commit (move) entity to a table.
 * This operation moves an entity from its current table to the specified
 * table. This may cause the following actions:
 * - Ctor for each component in the target table
 * - Move for each overlapping component
 * - Dtor for each component in the source table.
 * - OnAdd triggers for non-overlapping components in the target table
 * - OnRemove triggers for non-overlapping components in the source table.
 *
 * This operation is a faster than adding/removing components individually.
 *
 * The application must explicitly provide the difference in components between
 * tables as the added/removed parameters. This can usually be derived directly
 * from the result of ecs_table_add_id and esc_table_remove_id. These arrays are
 * required to properly execute OnAdd/OnRemove triggers.
 *
 * @param world The world.
 * @param entity The entity to commit.
 * @param record The entity's record (optional, providing it saves a lookup).
 * @param table The table to commit the entity to.
 * @return True if the entity got moved, false otherwise.
 */
FLECS_API
bool ecs_commit(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_record_t *record,
    ecs_table_t *table,
    const ecs_type_t *added,
    const ecs_type_t *removed);

/** Find record for entity. */
FLECS_API
ecs_record_t* ecs_record_find(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get component pointer from column/record. */
FLECS_API
void* ecs_record_get_column(
    const ecs_record_t *r,
    int32_t column,
    size_t c_size);

/** Search for component id in table type.
 * This operation returns the index of first occurrance of the id in the table
 * type. The id may be a wildcard.
 * 
 * When id_out is provided, the function will assign it with the found id. The
 * found id may be different from the provided id if it is a wildcard.
 * 
 * This is a constant time operation.
 * 
 * @param world The world.
 * @param table The table.
 * @param id The id to search for.
 * @param id_out If provided, it will be set to the found id (optional).
 * @return The index of the id in the table type.
 */
FLECS_API
int32_t ecs_search(
    const ecs_world_t *world,
    const ecs_table_t *table,
    ecs_id_t id,
    ecs_id_t *id_out);

/** Search for component id in table type starting from an offset.
 * This operation is the same as ecs_search, but starts searching from an offset
 * in the table type.
 * 
 * This operation is typically called in a loop where the resulting index is
 * used in the next iteration as offset:
 * 
 * int32_t index = -1;
 * while ((index = ecs_search_offset(world, table, offset, id, NULL))) {
 *   // do stuff
 * }
 * 
 * Depending on how the operation is used it is either linear or constant time.
 * When the id has the form (id) or (rel, *) and the operation is invoked as 
 * in the above example, it is guaranteed to be constant time.
 * 
 * If the provided id has the form (*, tgt) the operation takes linear time. The
 * reason for this is that ids for an target are not packed together, as they
 * are sorted relationship first.
 * 
 * If the id at the offset does not match the provided id, the operation will do
 * a linear search to find a matching id.
 * 
 * @param world The world.
 * @param table The table.
 * @param offset Offset from where to start searching.
 * @param id The id to search for.
 * @param id_out If provided, it will be set to the found id (optional).
 * @return The index of the id in the table type.
 */
FLECS_API
int32_t ecs_search_offset(
    const ecs_world_t *world,
    const ecs_table_t *table,
    int32_t offset,
    ecs_id_t id,
    ecs_id_t *id_out);

/** Search for component/relationship id in table type starting from an offset.
 * This operation is the same as ecs_search_offset, but has the additional
 * capability of traversing relationships to find a component. For example, if
 * an application wants to find a component for either the provided table or a
 * prefab (using the IsA relationship) of that table, it could use the operation 
 * like this:
 * 
 * int32_t index = ecs_search_relation(
 *   world,            // the world
 *   table,            // the table
 *   0,                // offset 0
 *   ecs_id(Position), // the component id
 *   EcsIsA,           // the relationship to traverse
 *   0,                // start at depth 0 (the table itself)
 *   0,                // no depth limit
 *   NULL,             // (optional) entity on which component was found
 *   NULL,             // see above
 *   NULL);            // internal type with information about matched id
 * 
 * The operation searches depth first. If a table type has 2 IsA relationships, the
 * operation will first search the IsA tree of the first relationship.
 * 
 * When choosing betwen ecs_search, ecs_search_offset and ecs_search_relation,
 * the simpler the function the better its performance.
 * 
 * @param world The world.
 * @param table The table.
 * @param offset Offset from where to start searching.
 * @param id The id to search for.
 * @param rel The relationship to traverse (optional).
 * @param flags Whether to search EcsSelf and/or EcsUp.
 * @param subject_out If provided, it will be set to the matched entity.
 * @param id_out If provided, it will be set to the found id (optional).
 * @param tr_out Internal datatype.
 * @return The index of the id in the table type.
 */
FLECS_API
int32_t ecs_search_relation(
    const ecs_world_t *world,
    const ecs_table_t *table,
    int32_t offset,
    ecs_id_t id,
    ecs_entity_t rel,
    ecs_flags32_t flags, /* EcsSelf and/or EcsUp */
    ecs_entity_t *subject_out,
    ecs_id_t *id_out,
    struct ecs_table_record_t **tr_out);

/** @} */

/**
 * @defgroup values Values
 * @brief Construct, destruct, copy and move dynamically created values.
 * @{
 */

/** Construct a value in existing storage 
 *
 * @param world The world.
 * @param type The type of the value to create.
 * @param ptr Pointer to a value of type 'type'
 * @return Zero if success, nonzero if failed.
 */
FLECS_API
int ecs_value_init(
    const ecs_world_t *world,
    ecs_entity_t type,
    void *ptr);

/** Construct a value in existing storage 
 *
 * @param world The world.
 * @param ti The type info of the type to create.
 * @param ptr Pointer to a value of type 'type'
 * @return Zero if success, nonzero if failed.
 */
FLECS_API
int ecs_value_init_w_type_info(
    const ecs_world_t *world,
    const ecs_type_info_t *ti,
    void *ptr);

/** Construct a value in new storage 
 * 
 * @param world The world.
 * @param type The type of the value to create.
 * @return Pointer to type if success, NULL if failed.
 */
FLECS_API
void* ecs_value_new(
    ecs_world_t *world,
    ecs_entity_t type);

/** Construct a value in new storage 
 * 
 * @param world The world.
 * @param ti The type info of the type to create.
 * @return Pointer to type if success, NULL if failed.
 */
void* ecs_value_new_w_type_info(
    ecs_world_t *world,
    const ecs_type_info_t *ti);

/** Destruct a value 
 * 
 * @param world The world.
 * @param ti Type info of the value to destruct.
 * @param ptr Pointer to constructed value of type 'type'.
 * @return Zero if success, nonzero if failed. 
 */
int ecs_value_fini_w_type_info(
    const ecs_world_t *world,
    const ecs_type_info_t *ti,
    void *ptr);

/** Destruct a value 
 * 
 * @param world The world.
 * @param type The type of the value to destruct.
 * @param ptr Pointer to constructed value of type 'type'.
 * @return Zero if success, nonzero if failed. 
 */
FLECS_API
int ecs_value_fini(
    const ecs_world_t *world,
    ecs_entity_t type,
    void* ptr);

/** Destruct a value, free storage
 * 
 * @param world The world.
 * @param type The type of the value to destruct.
 * @return Zero if success, nonzero if failed. 
 */
FLECS_API
int ecs_value_free(
    ecs_world_t *world,
    ecs_entity_t type,
    void* ptr);

/** Copy value.
 * 
 * @param world The world.
 * @param ti Type info of the value to copy.
 * @param dst Pointer to the storage to copy to.
 * @param src Pointer to the value to copy.
 * @return Zero if success, nonzero if failed. 
 */
FLECS_API
int ecs_value_copy_w_type_info(
    const ecs_world_t *world,
    const ecs_type_info_t *ti,
    void* dst,
    const void *src);

/** Copy value.
 * 
 * @param world The world.
 * @param type The type of the value to copy.
 * @param dst Pointer to the storage to copy to.
 * @param src Pointer to the value to copy.
 * @return Zero if success, nonzero if failed. 
 */
FLECS_API
int ecs_value_copy(
    const ecs_world_t *world,
    ecs_entity_t type,
    void* dst,
    const void *src);

/** Move value.
 * 
 * @param world The world.
 * @param ti Type info of the value to move.
 * @param dst Pointer to the storage to move to.
 * @param src Pointer to the value to move.
 * @return Zero if success, nonzero if failed. 
 */
int ecs_value_move_w_type_info(
    const ecs_world_t *world,
    const ecs_type_info_t *ti,
    void* dst,
    void *src);

/** Move value.
 * 
 * @param world The world.
 * @param type The type of the value to move.
 * @param dst Pointer to the storage to move to.
 * @param src Pointer to the value to move.
 * @return Zero if success, nonzero if failed. 
 */
int ecs_value_move(
    const ecs_world_t *world,
    ecs_entity_t type,
    void* dst,
    void *src);

/** Move construct value.
 * 
 * @param world The world.
 * @param ti Type info of the value to move.
 * @param dst Pointer to the storage to move to.
 * @param src Pointer to the value to move.
 * @return Zero if success, nonzero if failed. 
 */
int ecs_value_move_ctor_w_type_info(
    const ecs_world_t *world,
    const ecs_type_info_t *ti,
    void* dst,
    void *src);

/** Move construct value.
 * 
 * @param world The world.
 * @param type The type of the value to move.
 * @param dst Pointer to the storage to move to.
 * @param src Pointer to the value to move.
 * @return Zero if success, nonzero if failed. 
 */
int ecs_value_move_ctor(
    const ecs_world_t *world,
    ecs_entity_t type,
    void* dst,
    void *src);

/** @} */

/** @} */

/**
 * @defgroup c_addons Addons
 * @brief C APIs for addons.
 * 
 * \ingroup c
 * 
 * @{
 * @}
 */

/**
 * @file addons/flecs_c.h
 * @brief Extends the core API with convenience macros for C applications.
 */

#ifndef FLECS_C_
#define FLECS_C_

/**
 * @defgroup flecs_c Macro API
 * @brief Convenience macro's for C API
 * 
 * \ingroup c
 * @{
 */

/**
 * @defgroup flecs_c_creation Creation macro's
 * @brief Convenience macro's for creating entities, components and observers
 * @{
 */

/* Use for declaring entity, tag, prefab / any other entity identifier */
#define ECS_DECLARE(id)\
    ecs_entity_t id, ecs_id(id)

/** Forward declare an entity. */
#define ECS_ENTITY_DECLARE ECS_DECLARE

/** Define a forward declared entity.
 * 
 * Example:
 *   ECS_ENTITY_DEFINE(world, MyEntity, Position, Velocity);
 */
#define ECS_ENTITY_DEFINE(world, id_, ...) \
    { \
        ecs_entity_desc_t desc = {0}; \
        desc.id = id_; \
        desc.name = #id_; \
        desc.add_expr = #__VA_ARGS__; \
        id_ = ecs_entity_init(world, &desc); \
        ecs_id(id_) = id_; \
        ecs_assert(id_ != 0, ECS_INVALID_PARAMETER, NULL); \
    } \
    (void)id_; \
    (void)ecs_id(id_)

/** Declare & define an entity.
 *
 * Example:
 *   ECS_ENTITY(world, MyEntity, Position, Velocity);
 */
#define ECS_ENTITY(world, id, ...) \
    ecs_entity_t ecs_id(id); \
    ecs_entity_t id = 0; \
    ECS_ENTITY_DEFINE(world, id, __VA_ARGS__)

/** Forward declare a tag. */
#define ECS_TAG_DECLARE ECS_DECLARE

/** Define a forward declared tag.
 * 
 * Example:
 *   ECS_TAG_DEFINE(world, MyTag);
 */
#define ECS_TAG_DEFINE(world, id) ECS_ENTITY_DEFINE(world, id, 0)

/** Declare & define a tag.
 *
 * Example:
 *   ECS_TAG(world, MyTag);
 */
#define ECS_TAG(world, id) ECS_ENTITY(world, id, 0)

/** Forward declare a prefab. */
#define ECS_PREFAB_DECLARE ECS_DECLARE

/** Define a forward declared prefab.
 * 
 * Example:
 *   ECS_PREFAB_DEFINE(world, MyPrefab, Position, Velocity);
 */
#define ECS_PREFAB_DEFINE(world, id, ...) ECS_ENTITY_DEFINE(world, id, Prefab, __VA_ARGS__)

/** Declare & define a prefab.
 *
 * Example:
 *   ECS_PREFAB(world, MyPrefab, Position, Velocity);
 */
#define ECS_PREFAB(world, id, ...) ECS_ENTITY(world, id, Prefab, __VA_ARGS__)

/** Forward declare a component. */
#define ECS_COMPONENT_DECLARE(id)         ecs_entity_t ecs_id(id)

/** Define a forward declared component.
 * 
 * Example:
 *   ECS_COMPONENT_DEFINE(world, Position);
 */
#define ECS_COMPONENT_DEFINE(world, id_) \
    {\
        ecs_component_desc_t desc = {0}; \
        ecs_entity_desc_t edesc = {0}; \
        edesc.id = ecs_id(id_); \
        edesc.use_low_id = true; \
        edesc.name = #id_; \
        edesc.symbol = #id_; \
        desc.entity = ecs_entity_init(world, &edesc); \
        desc.type.size = ECS_SIZEOF(id_); \
        desc.type.alignment = ECS_ALIGNOF(id_); \
        ecs_id(id_) = ecs_component_init(world, &desc);\
    }\
    ecs_assert(ecs_id(id_) != 0, ECS_INVALID_PARAMETER, NULL)

/** Declare & define a component.
 *
 * Example:
 *   ECS_COMPONENT(world, Position);
 */
#define ECS_COMPONENT(world, id)\
    ecs_entity_t ecs_id(id) = 0;\
    ECS_COMPONENT_DEFINE(world, id);\
    (void)ecs_id(id)

/* Forward declare an observer. */
#define ECS_OBSERVER_DECLARE(id)         ecs_entity_t ecs_id(id)

/** Define a forward declared observer.
 * 
 * Example:
 *   ECS_OBSERVER_DEFINE(world, AddPosition, EcsOnAdd, Position);
 */
#define ECS_OBSERVER_DEFINE(world, id_, kind, ...)\
    {\
        ecs_observer_desc_t desc = {0};\
        ecs_entity_desc_t edesc = {0}; \
        edesc.id = ecs_id(id_); \
        edesc.name = #id_; \
        desc.entity = ecs_entity_init(world, &edesc); \
        desc.callback = id_;\
        desc.filter.expr = #__VA_ARGS__;\
        desc.events[0] = kind;\
        ecs_id(id_) = ecs_observer_init(world, &desc);\
        ecs_assert(ecs_id(id_) != 0, ECS_INVALID_PARAMETER, NULL);\
    }

/** Declare & define an observer.
 *
 * Example:
 *   ECS_OBSERVER(world, AddPosition, EcsOnAdd, Position);
 */
#define ECS_OBSERVER(world, id, kind, ...)\
    ecs_entity_t ecs_id(id) = 0; \
    ECS_OBSERVER_DEFINE(world, id, kind, __VA_ARGS__);\
    ecs_entity_t id = ecs_id(id);\
    (void)ecs_id(id);\
    (void)id

/** Shorthand for creating an entity with ecs_entity_init.
 *
 * Example:
 *   ecs_entity(world, {
 *     .name = "MyEntity"
 *   });
 */
#define ecs_entity(world, ...)\
    ecs_entity_init(world, &(ecs_entity_desc_t) __VA_ARGS__ )

/** Shorthand for creating a component with ecs_component_init.
 *
 * Example:
 *   ecs_component(world, {
 *     .type.size = 4,
 *     .type.alignment = 4
 *   });
 */
#define ecs_component(world, ...)\
    ecs_component_init(world, &(ecs_component_desc_t) __VA_ARGS__ )

/** Shorthand for creating a component from a type.
 *
 * Example:
 *   ecs_component_t(world, Position);
 */
#define ecs_component_t(world, T)\
    ecs_component_init(world, &(ecs_component_desc_t) { \
        .entity = ecs_entity(world, { \
            .name = #T, \
            .symbol = #T, \
            .use_low_id = true \
        }), \
        .type.size = ECS_SIZEOF(T), \
        .type.alignment = ECS_ALIGNOF(T) \
    })

/** Shorthand for creating a filter with ecs_filter_init.
 *
 * Example:
 *   ecs_filter(world, {
 *     .terms = {{ ecs_id(Position) }}
 *   });
 */
#define ecs_filter(world, ...)\
    ecs_filter_init(world, &(ecs_filter_desc_t) __VA_ARGS__ )

/** Shorthand for creating a query with ecs_query_init.
 *
 * Example:
 *   ecs_query(world, {
 *     .filter.terms = {{ ecs_id(Position) }}
 *   });
 */
#define ecs_query(world, ...)\
    ecs_query_init(world, &(ecs_query_desc_t) __VA_ARGS__ )

/** Shorthand for creating an observer with ecs_observer_init.
 *
 * Example:
 *   ecs_observer(world, {
 *     .filter.terms = {{ ecs_id(Position) }},
 *     .events = { EcsOnAdd },
 *     .callback = AddPosition
 *   });
 */
#define ecs_observer(world, ...)\
    ecs_observer_init(world, &(ecs_observer_desc_t) __VA_ARGS__ )

/** @} */

/**
 * @defgroup flecs_c_type_safe Type Safe API
 * @brief Macro's that wrap around core functions to provide a "type safe" API in C
 * @{
 */

/**
 * @defgroup flecs_c_entities Entity API
 * @{
 */

/**
 * @defgroup flecs_c_creation_deletion Creation & Deletion
 * @{
 */

#define ecs_new(world, T) ecs_new_w_id(world, ecs_id(T))

#define ecs_new_w_pair(world, first, second)\
    ecs_new_w_id(world, ecs_pair(first, second))

#define ecs_bulk_new(world, component, count)\
    ecs_bulk_new_w_id(world, ecs_id(component), count)

#define ecs_new_entity(world, n)\
    ecs_entity_init(world, &(ecs_entity_desc_t){\
        .name = n,\
    })

#define ecs_new_prefab(world, n)\
    ecs_entity_init(world, &(ecs_entity_desc_t){\
        .name = n,\
        .add = {EcsPrefab}\
    })

#define ecs_delete_children(world, parent)\
    ecs_delete_with(world, ecs_pair(EcsChildOf, parent))

/** @} */

/**
 * @defgroup flecs_c_adding_removing Adding & Removing
 * @{
 */

#define ecs_add(world, entity, T)\
    ecs_add_id(world, entity, ecs_id(T))

#define ecs_add_pair(world, subject, first, second)\
    ecs_add_id(world, subject, ecs_pair(first, second))


#define ecs_remove(world, entity, T)\
    ecs_remove_id(world, entity, ecs_id(T))

#define ecs_remove_pair(world, subject, first, second)\
    ecs_remove_id(world, subject, ecs_pair(first, second))


#define ecs_override(world, entity, T)\
    ecs_override_id(world, entity, ecs_id(T))

#define ecs_override_pair(world, subject, first, second)\
    ecs_override_id(world, subject, ecs_pair(first, second))

/** @} */

/**
 * @defgroup flecs_c_getting_setting Getting & Setting
 * @{
 */

#define ecs_set_ptr(world, entity, component, ptr)\
    ecs_set_id(world, entity, ecs_id(component), sizeof(component), ptr)

#define ecs_set(world, entity, component, ...)\
    ecs_set_id(world, entity, ecs_id(component), sizeof(component), &(component)__VA_ARGS__)

#define ecs_set_pair(world, subject, First, second, ...)\
    ecs_set_id(world, subject,\
        ecs_pair(ecs_id(First), second),\
        sizeof(First), &(First)__VA_ARGS__)

#define ecs_set_pair_second(world, subject, first, Second, ...)\
    ecs_set_id(world, subject,\
        ecs_pair(first, ecs_id(Second)),\
        sizeof(Second), &(Second)__VA_ARGS__)

#define ecs_set_pair_object ecs_set_pair_second

#define ecs_set_override(world, entity, T, ...)\
    ecs_add_id(world, entity, ECS_OVERRIDE | ecs_id(T));\
    ecs_set(world, entity, T, __VA_ARGS__)

#define ecs_emplace(world, entity, T)\
    (ECS_CAST(T*, ecs_emplace_id(world, entity, ecs_id(T))))

#define ecs_emplace_pair(world, entity, First, second)\
    (ECS_CAST(First*, ecs_emplace_id(world, entity, ecs_pair_t(First, second))))

#define ecs_get(world, entity, T)\
    (ECS_CAST(const T*, ecs_get_id(world, entity, ecs_id(T))))

#define ecs_get_pair(world, subject, First, second)\
    (ECS_CAST(const First*, ecs_get_id(world, subject,\
        ecs_pair(ecs_id(First), second))))

#define ecs_get_pair_second(world, subject, first, Second)\
    (ECS_CAST(const Second*, ecs_get_id(world, subject,\
        ecs_pair(first, ecs_id(Second)))))

#define ecs_get_pair_object ecs_get_pair_second

#define ecs_record_get(world, record, T)\
    (ECS_CAST(const T*, ecs_record_get_id(world, record, ecs_id(T))))

#define ecs_record_has(world, record, T)\
    (ecs_record_has_id(world, record, ecs_id(T)))

#define ecs_record_get_pair(world, record, First, second)\
    (ECS_CAST(const First*, ecs_record_get_id(world, record, \
        ecs_pair(ecs_id(First), second))))

#define ecs_record_get_pair_second(world, record, first, Second)\
    (ECS_CAST(const Second*, ecs_record_get_id(world, record,\
        ecs_pair(first, ecs_id(Second)))))

#define ecs_record_get_mut(world, record, T)\
    (ECS_CAST(T*, ecs_record_get_mut_id(world, record, ecs_id(T))))

#define ecs_record_get_mut_pair(world, record, First, second)\
    (ECS_CAST(First*, ecs_record_get_mut_id(world, record, \
        ecs_pair(ecs_id(First), second))))

#define ecs_record_get_mut_pair_second(world, record, first, Second)\
    (ECS_CAST(Second*, ecs_record_get_mut_id(world, record,\
        ecs_pair(first, ecs_id(Second)))))

#define ecs_record_get_mut_pair_object ecs_record_get_mut_pair_second

#define ecs_ref_init(world, entity, T)\
    ecs_ref_init_id(world, entity, ecs_id(T))

#define ecs_ref_get(world, ref, T)\
    (ECS_CAST(const T*, ecs_ref_get_id(world, ref, ecs_id(T))))

#define ecs_get_mut(world, entity, T)\
    (ECS_CAST(T*, ecs_get_mut_id(world, entity, ecs_id(T))))

#define ecs_get_mut_pair(world, subject, First, second)\
    (ECS_CAST(First*, ecs_get_mut_id(world, subject,\
        ecs_pair(ecs_id(First), second))))

#define ecs_get_mut_pair_second(world, subject, first, Second)\
    (ECS_CAST(Second*, ecs_get_mut_id(world, subject,\
        ecs_pair(first, ecs_id(Second)))))

#define ecs_get_mut_pair_object ecs_get_mut_pair_second

#define ecs_modified(world, entity, component)\
    ecs_modified_id(world, entity, ecs_id(component))

#define ecs_modified_pair(world, subject, first, second)\
    ecs_modified_id(world, subject, ecs_pair(first, second))

/** @} */

/**
 * @defgroup flecs_c_singletons Singletons
 * @{
 */

#define ecs_singleton_add(world, comp)\
    ecs_add(world, ecs_id(comp), comp)

#define ecs_singleton_remove(world, comp)\
    ecs_remove(world, ecs_id(comp), comp)

#define ecs_singleton_get(world, comp)\
    ecs_get(world, ecs_id(comp), comp)

#define ecs_singleton_set_ptr(world, comp, ptr)\
    ecs_set_ptr(world, ecs_id(comp), comp, ptr)

#define ecs_singleton_set(world, comp, ...)\
    ecs_set(world, ecs_id(comp), comp, __VA_ARGS__)

#define ecs_singleton_get_mut(world, comp)\
    ecs_get_mut(world, ecs_id(comp), comp)

#define ecs_singleton_modified(world, comp)\
    ecs_modified(world, ecs_id(comp), comp)

/** @} */

/**
 * @defgroup flecs_c_has Has, Owns, Shares
 * @{
 */

#define ecs_has(world, entity, T)\
    ecs_has_id(world, entity, ecs_id(T))

#define ecs_has_pair(world, entity, first, second)\
    ecs_has_id(world, entity, ecs_pair(first, second))

#define ecs_owns_pair(world, entity, first, second)\
    ecs_owns_id(world, entity, ecs_pair(first, second))

#define ecs_owns(world, entity, T)\
    ecs_owns_id(world, entity, ecs_id(T))

#define ecs_shares_id(world, entity, id)\
    (ecs_search_relation(world, ecs_get_table(world, entity), 0, ecs_id(id), \
        EcsIsA, 1, 0, 0, 0, 0) != -1)

#define ecs_shares_pair(world, entity, first, second)\
    (ecs_shares_id(world, entity, ecs_pair(first, second)))

#define ecs_shares(world, entity, T)\
    (ecs_shares_id(world, entity, ecs_id(T)))

/** @} */

/**
 * @defgroup flecs_c_enable_disable Enabling & Disabling
 * @{
 */

#define ecs_enable_component(world, entity, T, enable)\
    ecs_enable_id(world, entity, ecs_id(T), enable)

#define ecs_is_enabled_component(world, entity, T)\
    ecs_is_enabled_id(world, entity, ecs_id(T))

#define ecs_enable_pair(world, entity, First, second, enable)\
    ecs_enable_id(world, entity, ecs_pair(ecs_id(First), second), enable)

#define ecs_is_enabled_pair(world, entity, First, second)\
    ecs_is_enabled_id(world, entity, ecs_pair(ecs_id(First), second))

/** @} */

/**
 * @defgroup flecs_c_entity_names Entity Names
 * @{
 */

#define ecs_lookup_path(world, parent, path)\
    ecs_lookup_path_w_sep(world, parent, path, ".", NULL, true)

#define ecs_lookup_fullpath(world, path)\
    ecs_lookup_path_w_sep(world, 0, path, ".", NULL, true)

#define ecs_get_path(world, parent, child)\
    ecs_get_path_w_sep(world, parent, child, ".", NULL)

#define ecs_get_fullpath(world, child)\
    ecs_get_path_w_sep(world, 0, child, ".", NULL)

#define ecs_get_fullpath_buf(world, child, buf)\
    ecs_get_path_w_sep_buf(world, 0, child, ".", NULL, buf)

#define ecs_new_from_path(world, parent, path)\
    ecs_new_from_path_w_sep(world, parent, path, ".", NULL)

#define ecs_new_from_fullpath(world, path)\
    ecs_new_from_path_w_sep(world, 0, path, ".", NULL)

#define ecs_add_path(world, entity, parent, path)\
    ecs_add_path_w_sep(world, entity, parent, path, ".", NULL)

#define ecs_add_fullpath(world, entity, path)\
    ecs_add_path_w_sep(world, entity, 0, path, ".", NULL)

/** @} */

/** @} */

/**
 * @defgroup flecs_c_components Component API
 * @{
 */

#define ecs_set_hooks(world, T, ...)\
    ecs_set_hooks_id(world, ecs_id(T), &(ecs_type_hooks_t)__VA_ARGS__)

#define ecs_get_hooks(world, T)\
    ecs_get_hooks_id(world, ecs_id(T));

/** Declare a constructor.
 * Example:
 *   ECS_CTOR(MyType, ptr, { ptr->value = NULL; });
 */
#define ECS_CTOR(type, var, ...)\
    ECS_XTOR_IMPL(type, ctor, var, __VA_ARGS__)

/** Declare a destructor.
 * Example:
 *   ECS_DTOR(MyType, ptr, { free(ptr->value); });
 */
#define ECS_DTOR(type, var, ...)\
    ECS_XTOR_IMPL(type, dtor, var, __VA_ARGS__)

/** Declare a copy action.
 * Example:
 *   ECS_COPY(MyType, dst, src, { dst->value = strdup(src->value); });
 */
#define ECS_COPY(type, dst_var, src_var, ...)\
    ECS_COPY_IMPL(type, dst_var, src_var, __VA_ARGS__)

/** Declare a move action.
 * Example:
 *   ECS_MOVE(MyType, dst, src, { dst->value = src->value; src->value = 0; });
 */
#define ECS_MOVE(type, dst_var, src_var, ...)\
    ECS_MOVE_IMPL(type, dst_var, src_var, __VA_ARGS__)

/** Declare component hooks.
 * Example:
 *   ECS_ON_SET(MyType, ptr, { printf("%d\n", ptr->value); });
 */
#define ECS_ON_ADD(type, ptr, ...)\
    ECS_HOOK_IMPL(type, ecs_on_add(type), ptr, __VA_ARGS__)
#define ECS_ON_REMOVE(type, ptr, ...)\
    ECS_HOOK_IMPL(type, ecs_on_remove(type), ptr, __VA_ARGS__)
#define ECS_ON_SET(type, ptr, ...)\
    ECS_HOOK_IMPL(type, ecs_on_set(type), ptr, __VA_ARGS__)

/* Map from typename to function name of component lifecycle action */
#define ecs_ctor(type) type##_ctor
#define ecs_dtor(type) type##_dtor
#define ecs_copy(type) type##_copy
#define ecs_move(type) type##_move
#define ecs_on_set(type) type##_on_set
#define ecs_on_add(type) type##_on_add
#define ecs_on_remove(type) type##_on_remove

/** @} */

/**
 * @defgroup flecs_c_ids Id API
 * @{
 */

#define ecs_count(world, type)\
    ecs_count_id(world, ecs_id(type))

/** @} */

/**
 * @defgroup flecs_c_iterators Iterator API
 * @{
 */

#define ecs_field(it, T, index)\
    (ECS_CAST(T*, ecs_field_w_size(it, sizeof(T), index)))

/** @} */

/**
 * @defgroup flecs_c_tables Table API
 * @{
 */

#define ecs_table_get(world, table, T, offset)\
    (ECS_CAST(T*, ecs_table_get_id(world, table, ecs_id(T), offset)))

#define ecs_table_get_pair(world, table, First, second, offset)\
    (ECS_CAST(First*, ecs_table_get_id(world, table, ecs_pair(ecs_id(First), second), offset)))

#define ecs_table_get_pair_second(world, table, first, Second, offset)\
    (ECS_CAST(Second*, ecs_table_get_id(world, table, ecs_pair(first, ecs_id(Second)), offset)))

/** @} */

/**
 * @defgroup flecs_c_values Value API
 * @{
 */

#define ecs_value(T, ptr) ((ecs_value_t){ecs_id(T), ptr})
#define ecs_value_new_t(world, T) ecs_value_new(world, ecs_id(T))

/** @} */

/** @} */

/**
 * @defgroup flecs_c_table_sorting Table sorting
 * @brief Convenience macro's for sorting tables.
 * 
 * @{
 */
#define ecs_sort_table(id) ecs_id(id##_sort_table)

#define ecs_compare(id) ecs_id(id##_compare_fn)

/* Declare efficient table sorting operation that uses provided compare function.
 * For best results use LTO or make the function body visible in the same compilation unit.
 * Variadic arguments are prepended before generated functions, use it to declare static
 *   or exported functions.
 * Parameters of the comparison function:
 *   ecs_entity_t e1, const void* ptr1,
 *   ecs_entity_t e2, const void* ptr2
 * Parameters of the sort functions:
 *   ecs_world_t *world
 *   ecs_table_t *table
 *   ecs_entity_t *entities
 *   void *ptr
 *   int32_t elem_size
 *   int32_t lo
 *   int32_t hi
 *   ecs_order_by_action_t order_by - Pointer to the original comparison function. You are not supposed to use it.
 * Example:
 *   int CompareMyType(ecs_entity_t e1, const void* ptr1, ecs_entity_t e2, const void* ptr2) { const MyType* p1 = ptr1; const MyType* p2 = ptr2; return p1->value - p2->value; }
 *   ECS_SORT_TABLE_WITH_COMPARE(MyType, MyCustomCompare, CompareMyType)
 */
#define ECS_SORT_TABLE_WITH_COMPARE(id, op_name, compare_fn, ...) \
    static int32_t ECS_CONCAT(op_name, _partition)( \
        ecs_world_t *world, \
        ecs_table_t *table, \
        ecs_entity_t *entities, \
        void *ptr, \
        int32_t elem_size, \
        int32_t lo, \
        int32_t hi, \
        ecs_order_by_action_t order_by) \
    { \
        (void)(order_by); \
        int32_t p = (hi + lo) / 2; \
        void *pivot = ECS_ELEM(ptr, elem_size, p); \
        ecs_entity_t pivot_e = entities[p]; \
        int32_t i = lo - 1, j = hi + 1; \
        void *el; \
    repeat: \
        { \
            do { \
                i ++; \
                el = ECS_ELEM(ptr, elem_size, i); \
            } while ( compare_fn(entities[i], el, pivot_e, pivot) < 0); \
            do { \
                j --; \
                el = ECS_ELEM(ptr, elem_size, j); \
            } while ( compare_fn(entities[j], el, pivot_e, pivot) > 0); \
            if (i >= j) { \
                return j; \
            } \
            ecs_table_swap_rows(world, table, i, j); \
            if (p == i) { \
                pivot = ECS_ELEM(ptr, elem_size, j); \
                pivot_e = entities[j]; \
            } else if (p == j) { \
                pivot = ECS_ELEM(ptr, elem_size, i); \
                pivot_e = entities[i]; \
            } \
            goto repeat; \
        } \
    } \
    __VA_ARGS__ void op_name( \
        ecs_world_t *world, \
        ecs_table_t *table, \
        ecs_entity_t *entities, \
        void *ptr, \
        int32_t size, \
        int32_t lo, \
        int32_t hi, \
        ecs_order_by_action_t order_by) \
    { \
        if ((hi - lo) < 1)  { \
            return; \
        } \
        int32_t p = ECS_CONCAT(op_name, _partition)(world, table, entities, ptr, size, lo, hi, order_by); \
        op_name(world, table, entities, ptr, size, lo, p, order_by); \
        op_name(world, table, entities, ptr, size, p + 1, hi, order_by); \
    }

/* Declare efficient table sorting operation that uses default component comparison operator.
 * For best results use LTO or make the comparison operator visible in the same compilation unit.
 * Variadic arguments are prepended before generated functions, use it to declare static
 *   or exported functions.
 * Example:
 *   ECS_COMPARE(MyType, { const MyType* p1 = ptr1; const MyType* p2 = ptr2; return p1->value - p2->value; });
 *   ECS_SORT_TABLE(MyType)
 */
#define ECS_SORT_TABLE(id, ...) \
    ECS_SORT_TABLE_WITH_COMPARE(id, ecs_sort_table(id), ecs_compare(id), __VA_ARGS__)

/* Declare component comparison operations.
 * Parameters:
 *   ecs_entity_t e1, const void* ptr1,
 *   ecs_entity_t e2, const void* ptr2
 * Example:
 *   ECS_COMPARE(MyType, { const MyType* p1 = ptr1; const MyType* p2 = ptr2; return p1->value - p2->value; });
 */
#define ECS_COMPARE(id, ...) \
    int ecs_compare(id)(ecs_entity_t e1, const void* ptr1, ecs_entity_t e2, const void* ptr2) { \
        __VA_ARGS__ \
    }

/** @} */

/**
 * @defgroup flecs_c_misc Misc
 * @brief Misc convenience macro's.
 * 
 * @{
 */

#define ecs_isa(e)       ecs_pair(EcsIsA, e)
#define ecs_childof(e)   ecs_pair(EcsChildOf, e)
#define ecs_dependson(e) ecs_pair(EcsDependsOn, e)

#define ecs_query_new(world, q_expr)\
    ecs_query_init(world, &(ecs_query_desc_t){\
        .filter.expr = q_expr\
    })

#define ecs_rule_new(world, q_expr)\
    ecs_rule_init(world, &(ecs_filter_desc_t){\
        .expr = q_expr\
    })

/** @} */

/** @} */

#endif // FLECS_C_



/**
 * @file addons.h
 * @brief Include enabled addons.
 *
 * This file should only be included by the main flecs.h header.
 */

#ifndef FLECS_ADDONS_H
#define FLECS_ADDONS_H

/* Blacklist macros */
#ifdef FLECS_NO_CPP
#undef FLECS_CPP
#endif
#ifdef FLECS_NO_MODULE
#undef FLECS_MODULE
#endif
#ifdef FLECS_NO_PARSER
#undef FLECS_PARSER
#endif
#ifdef FLECS_NO_PLECS
#undef FLECS_PLECS
#endif
#ifdef FLECS_NO_RULES
#undef FLECS_RULES
#endif
#ifdef FLECS_NO_SNAPSHOT
#undef FLECS_SNAPSHOT
#endif
#ifdef FLECS_NO_MONITOR
#undef FLECS_MONITOR
#endif
#ifdef FLECS_NO_STATS
#undef FLECS_STATS
#endif
#ifdef FLECS_NO_SYSTEM
#undef FLECS_SYSTEM
#endif
#ifdef FLECS_NO_PIPELINE
#undef FLECS_PIPELINE
#endif
#ifdef FLECS_NO_TIMER
#undef FLECS_TIMER
#endif
#ifdef FLECS_NO_META
#undef FLECS_META
#endif
#ifdef FLECS_NO_META_C
#undef FLECS_META_C
#endif
#ifdef FLECS_NO_UNITS
#undef FLECS_UNITS
#endif
#ifdef FLECS_NO_EXPR
#undef FLECS_EXPR
#endif
#ifdef FLECS_NO_JSON
#undef FLECS_JSON
#endif
#ifdef FLECS_NO_DOC
#undef FLECS_DOC
#endif
#ifdef FLECS_NO_COREDOC
#undef FLECS_COREDOC
#endif
#ifdef FLECS_NO_LOG
#undef FLECS_LOG
#endif
#ifdef FLECS_NO_APP
#undef FLECS_APP
#endif
#ifdef FLECS_NO_OS_API_IMPL
#undef FLECS_OS_API_IMPL
#endif
#ifdef FLECS_NO_HTTP
#undef FLECS_HTTP
#endif
#ifdef FLECS_NO_REST
#undef FLECS_REST
#endif
#ifdef FLECS_NO_JOURNAL
#undef FLECS_JOURNAL
#endif

/* Always included, if disabled functions are replaced with dummy macros */
/**
 * @file addons/journal.h
 * @brief Journaling addon that logs API functions.
 *
 * The journaling addon traces API calls. The trace is formatted as runnable
 * C code, which allows for (partially) reproducing the behavior of an app
 * with the journaling trace.
 * 
 * The journaling addon is disabled by default. Enabling it can have a 
 * significant impact on performance.
 */

#ifdef FLECS_JOURNAL

#ifndef FLECS_LOG
#define FLECS_LOG
#endif

#ifndef FLECS_JOURNAL_H
#define FLECS_JOURNAL_H

/**
 * @defgroup c_addons_journal Journal
 * @brief Journaling addon (disabled by default).
 * 
 * \ingroup c_addons
 * @{
 */

/* Trace when log level is at or higher than level */
#define FLECS_JOURNAL_LOG_LEVEL (0)


/* Journaling API, meant to be used by internals. */

typedef enum ecs_journal_kind_t {
    EcsJournalNew,
    EcsJournalMove,
    EcsJournalClear,
    EcsJournalDelete,
    EcsJournalDeleteWith,
    EcsJournalRemoveAll,
    EcsJournalTableEvents
} ecs_journal_kind_t;

FLECS_DBG_API
void flecs_journal_begin(
    ecs_world_t *world,
    ecs_journal_kind_t kind,
    ecs_entity_t entity,
    ecs_type_t *add,
    ecs_type_t *remove);

FLECS_DBG_API
void flecs_journal_end(void);

#define flecs_journal(...)\
    flecs_journal_begin(__VA_ARGS__);\
    flecs_journal_end();

#endif // FLECS_JOURNAL_H
#else
#define flecs_journal_begin(...)
#define flecs_journal_end(...)
#define flecs_journal(...)

/** @} */

#endif // FLECS_JOURNAL

/**
 * @file addons/log.h
 * @brief Logging addon.
 * 
 * The logging addon provides an API for (debug) tracing and reporting errors
 * at various levels. When enabled, the logging addon can provide more detailed
 * information about the state of the ECS and any errors that may occur.
 * 
 * The logging addon can be disabled to reduce footprint of the library, but
 * limits information logged to only file, line and error code.
 * 
 * When enabled the logging addon can be configured to exclude levels of tracing
 * from the build to reduce the impact on performance. By default all debug 
 * tracing is enabled for debug builds, tracing is enabled at release builds.
 * 
 * Applications can change the logging level at runtime with ecs_log_set_level,
 * but what is actually logged depends on what is compiled (when compiled 
 * without debug tracing, setting the runtime level to debug won't have an 
 * effect).
 * 
 * The logging addon uses the OS API log_ function for all tracing.
 * 
 * Note that even when the logging addon is not enabled, its header/source must
 * be included in a build. To prevent unused variable warnings in the code, some
 * API functions are included when the addon is disabled, but have empty bodies.
 */

#ifndef FLECS_LOG_H
#define FLECS_LOG_H


#ifdef FLECS_LOG

/**
 * @defgroup c_addons_log Log
 * @brief Logging functions.
 * 
 * \ingroup c_addons
 * @{
 */

////////////////////////////////////////////////////////////////////////////////
//// Tracing
////////////////////////////////////////////////////////////////////////////////

FLECS_API
void ecs_deprecated_(
    const char *file, 
    int32_t line, 
    const char *msg);

/** Increase log stack.
 * This operation increases the indent_ value of the OS API and can be useful to
 * make nested behavior more visible.
 * 
 * @param level The log level.
 */
FLECS_API
void ecs_log_push_(int32_t level);

/** Decrease log stack.
 * This operation decreases the indent_ value of the OS API and can be useful to
 * make nested behavior more visible.
 * 
 * @param level The log level.
 */
FLECS_API
void ecs_log_pop_(int32_t level);

/** Should current level be logged.
 * This operation returns true when the specified log level should be logged 
 * with the current log level.
 *
 * @param level The log level to check for.
 * @return Whether logging is enabled for the current level.
 */
FLECS_API
bool ecs_should_log(int32_t level);

////////////////////////////////////////////////////////////////////////////////
//// Error reporting
////////////////////////////////////////////////////////////////////////////////

/** Get description for error code */
FLECS_API
const char* ecs_strerror(
    int32_t error_code);

#else // FLECS_LOG

////////////////////////////////////////////////////////////////////////////////
//// Dummy macros for when logging is disabled
////////////////////////////////////////////////////////////////////////////////

#define ecs_deprecated_(file, line, msg)\
    (void)file;\
    (void)line;\
    (void)msg

#define ecs_log_push_(level)
#define ecs_log_pop_(level)
#define ecs_should_log(level) false

#define ecs_strerror(error_code)\
    (void)error_code

#endif // FLECS_LOG


////////////////////////////////////////////////////////////////////////////////
//// Logging functions (do nothing when logging is enabled)
////////////////////////////////////////////////////////////////////////////////

FLECS_API
void ecs_print_(
    int32_t level,
    const char *file,
    int32_t line,
    const char *fmt,
    ...);

FLECS_API
void ecs_printv_(
    int level,
    const char *file,
    int32_t line,
    const char *fmt,
    va_list args);

FLECS_API
void ecs_log_(
    int32_t level,
    const char *file,
    int32_t line,
    const char *fmt,
    ...);

FLECS_API
void ecs_logv_(
    int level,
    const char *file,
    int32_t line,
    const char *fmt,
    va_list args);

FLECS_API
void ecs_abort_(
    int32_t error_code,
    const char *file,
    int32_t line,
    const char *fmt,
    ...);

FLECS_API
void ecs_assert_log_(
    int32_t error_code,
    const char *condition_str,
    const char *file,
    int32_t line,
    const char *fmt,
    ...);

FLECS_API
void ecs_parser_error_(
    const char *name,
    const char *expr, 
    int64_t column,
    const char *fmt,
    ...);

FLECS_API
void ecs_parser_errorv_(
    const char *name,
    const char *expr, 
    int64_t column,
    const char *fmt,
    va_list args);


////////////////////////////////////////////////////////////////////////////////
//// Logging macros
////////////////////////////////////////////////////////////////////////////////

#ifndef FLECS_LEGACY /* C89 doesn't support variadic macros */

/* Base logging function. Accepts a custom level */
#define ecs_print(level, ...)\
    ecs_print_(level, __FILE__, __LINE__, __VA_ARGS__)

#define ecs_printv(level, fmt, args)\
    ecs_printv_(level, __FILE__, __LINE__, fmt, args)

#define ecs_log(level, ...)\
    ecs_log_(level, __FILE__, __LINE__, __VA_ARGS__)

#define ecs_logv(level, fmt, args)\
    ecs_logv_(level, __FILE__, __LINE__, fmt, args)

/* Tracing. Used for logging of infrequent events  */
#define ecs_trace_(file, line, ...) ecs_log_(0, file, line, __VA_ARGS__)
#define ecs_trace(...) ecs_trace_(__FILE__, __LINE__, __VA_ARGS__)

/* Warning. Used when an issue occurs, but operation is successful */
#define ecs_warn_(file, line, ...) ecs_log_(-2, file, line, __VA_ARGS__)
#define ecs_warn(...) ecs_warn_(__FILE__, __LINE__, __VA_ARGS__)

/* Error. Used when an issue occurs, and operation failed. */
#define ecs_err_(file, line, ...) ecs_log_(-3, file, line, __VA_ARGS__)
#define ecs_err(...) ecs_err_(__FILE__, __LINE__, __VA_ARGS__)

/* Fatal. Used when an issue occurs, and the application cannot continue. */
#define ecs_fatal_(file, line, ...) ecs_log_(-4, file, line, __VA_ARGS__)
#define ecs_fatal(...) ecs_fatal_(__FILE__, __LINE__, __VA_ARGS__)

/* Optionally include warnings about using deprecated features */
#ifndef FLECS_NO_DEPRECATED_WARNINGS
#define ecs_deprecated(...)\
    ecs_deprecated_(__FILE__, __LINE__, __VA_ARGS__)
#else
#define ecs_deprecated(...)
#endif // FLECS_NO_DEPRECATED_WARNINGS

/* If no tracing verbosity is defined, pick default based on build config */
#if !(defined(FLECS_LOG_0) || defined(FLECS_LOG_1) || defined(FLECS_LOG_2) || defined(FLECS_LOG_3))
#if !defined(FLECS_NDEBUG)
#define FLECS_LOG_3 /* Enable all tracing in debug mode. May slow things down */
#else
#define FLECS_LOG_0 /* Only enable infrequent tracing in release mode */
#endif // !defined(FLECS_NDEBUG)
#endif // !(defined(FLECS_LOG_0) || defined(FLECS_LOG_1) || defined(FLECS_LOG_2) || defined(FLECS_LOG_3))


/* Define/undefine macros based on compiled-in tracing level. This can optimize
 * out tracing statements from a build, which improves performance. */

#if defined(FLECS_LOG_3) /* All debug tracing enabled */
#define ecs_dbg_1(...) ecs_log(1, __VA_ARGS__);
#define ecs_dbg_2(...) ecs_log(2, __VA_ARGS__);
#define ecs_dbg_3(...) ecs_log(3, __VA_ARGS__);

#define ecs_log_push_1() ecs_log_push_(1);
#define ecs_log_push_2() ecs_log_push_(2);
#define ecs_log_push_3() ecs_log_push_(3);

#define ecs_log_pop_1() ecs_log_pop_(1);
#define ecs_log_pop_2() ecs_log_pop_(2);
#define ecs_log_pop_3() ecs_log_pop_(3);

#define ecs_should_log_1() ecs_should_log(1)
#define ecs_should_log_2() ecs_should_log(2)
#define ecs_should_log_3() ecs_should_log(3)

#define FLECS_LOG_2
#define FLECS_LOG_1
#define FLECS_LOG_0

#elif defined(FLECS_LOG_2) /* Level 2 and below debug tracing enabled */
#define ecs_dbg_1(...) ecs_log(1, __VA_ARGS__);
#define ecs_dbg_2(...) ecs_log(2, __VA_ARGS__);
#define ecs_dbg_3(...)

#define ecs_log_push_1() ecs_log_push_(1);
#define ecs_log_push_2() ecs_log_push_(2);
#define ecs_log_push_3()

#define ecs_log_pop_1() ecs_log_pop_(1);
#define ecs_log_pop_2() ecs_log_pop_(2);
#define ecs_log_pop_3()

#define ecs_should_log_1() ecs_should_log(1)
#define ecs_should_log_2() ecs_should_log(2)
#define ecs_should_log_3() false

#define FLECS_LOG_1
#define FLECS_LOG_0

#elif defined(FLECS_LOG_1) /* Level 1 debug tracing enabled */
#define ecs_dbg_1(...) ecs_log(1, __VA_ARGS__);
#define ecs_dbg_2(...)
#define ecs_dbg_3(...)

#define ecs_log_push_1() ecs_log_push_(1);
#define ecs_log_push_2()
#define ecs_log_push_3()

#define ecs_log_pop_1() ecs_log_pop_(1);
#define ecs_log_pop_2()
#define ecs_log_pop_3()

#define ecs_should_log_1() ecs_should_log(1)
#define ecs_should_log_2() false
#define ecs_should_log_3() false

#define FLECS_LOG_0

#elif defined(FLECS_LOG_0) /* No debug tracing enabled */
#define ecs_dbg_1(...)
#define ecs_dbg_2(...)
#define ecs_dbg_3(...)

#define ecs_log_push_1()
#define ecs_log_push_2()
#define ecs_log_push_3()

#define ecs_log_pop_1()
#define ecs_log_pop_2()
#define ecs_log_pop_3()

#define ecs_should_log_1() false
#define ecs_should_log_2() false
#define ecs_should_log_3() false

#else /* No tracing enabled */
#undef ecs_trace
#define ecs_trace(...)
#define ecs_dbg_1(...)
#define ecs_dbg_2(...)
#define ecs_dbg_3(...)

#define ecs_log_push_1()
#define ecs_log_push_2()
#define ecs_log_push_3()

#define ecs_log_pop_1()
#define ecs_log_pop_2()
#define ecs_log_pop_3()

#endif // defined(FLECS_LOG_3)

/* Default debug tracing is at level 1 */
#define ecs_dbg ecs_dbg_1

/* Default level for push/pop is 0 */
#define ecs_log_push() ecs_log_push_(0)
#define ecs_log_pop() ecs_log_pop_(0)

/** Abort.
 * Unconditionally aborts process. */
#define ecs_abort(error_code, ...)\
    ecs_abort_(error_code, __FILE__, __LINE__, __VA_ARGS__);\
    ecs_os_abort(); abort(); /* satisfy compiler/static analyzers */

/** Assert. 
 * Aborts if condition is false, disabled in debug mode. */
#if defined(FLECS_NDEBUG) && !defined(FLECS_KEEP_ASSERT)
#define ecs_assert(condition, error_code, ...)
#else
#define ecs_assert(condition, error_code, ...)\
    if (!(condition)) {\
        ecs_assert_log_(error_code, #condition, __FILE__, __LINE__, __VA_ARGS__);\
        ecs_os_abort();\
    }\
    assert(condition) /* satisfy compiler/static analyzers */
#endif // FLECS_NDEBUG

#define ecs_assert_var(var, error_code, ...)\
    ecs_assert(var, error_code, __VA_ARGS__);\
    (void)var

/** Debug assert.
 * Assert that is only valid in debug mode (ignores FLECS_KEEP_ASSERT) */
#ifndef FLECS_NDEBUG
#define ecs_dbg_assert(condition, error_code, ...) ecs_assert(condition, error_code, __VA_ARGS__)
#else
#define ecs_dbg_assert(condition, error_code, ...)
#endif

/** Sanitize assert.
 * Assert that is only valid in sanitized mode (ignores FLECS_KEEP_ASSERT) */
#ifdef FLECS_SANITIZE
#define ecs_san_assert(condition, error_code, ...) ecs_assert(condition, error_code, __VA_ARGS__)
#else
#define ecs_san_assert(condition, error_code, ...)
#endif


/* Silence dead code/unused label warnings when compiling without checks. */
#define ecs_dummy_check\
    if ((false)) {\
        goto error;\
    }

/** Check.
 * goto error if condition is false. */
#if defined(FLECS_NDEBUG) && !defined(FLECS_KEEP_ASSERT)
#define ecs_check(condition, error_code, ...) ecs_dummy_check
#else
#ifdef FLECS_SOFT_ASSERT
#define ecs_check(condition, error_code, ...)\
    if (!(condition)) {\
        ecs_assert_log_(error_code, #condition, __FILE__, __LINE__, __VA_ARGS__);\
        goto error;\
    }
#else // FLECS_SOFT_ASSERT
#define ecs_check(condition, error_code, ...)\
    ecs_assert(condition, error_code, __VA_ARGS__);\
    ecs_dummy_check
#endif
#endif // FLECS_NDEBUG

/** Panic.
 * goto error when FLECS_SOFT_ASSERT is defined, otherwise abort */
#if defined(FLECS_NDEBUG) && !defined(FLECS_KEEP_ASSERT)
#define ecs_throw(error_code, ...) ecs_dummy_check
#else
#ifdef FLECS_SOFT_ASSERT
#define ecs_throw(error_code, ...)\
    ecs_abort_(error_code, __FILE__, __LINE__, __VA_ARGS__);\
    goto error;
#else
#define ecs_throw(error_code, ...)\
    ecs_abort(error_code, __VA_ARGS__);\
    ecs_dummy_check
#endif
#endif // FLECS_NDEBUG

/** Parser error */
#define ecs_parser_error(name, expr, column, ...)\
    ecs_parser_error_(name, expr, column, __VA_ARGS__)

#define ecs_parser_errorv(name, expr, column, fmt, args)\
    ecs_parser_errorv_(name, expr, column, fmt, args)

#endif // FLECS_LEGACY


////////////////////////////////////////////////////////////////////////////////
//// Functions that are always available
////////////////////////////////////////////////////////////////////////////////

/** Enable or disable log.
 * This will enable builtin log. For log to work, it will have to be
 * compiled in which requires defining one of the following macros:
 *
 * FLECS_LOG_0 - All log is disabled
 * FLECS_LOG_1 - Enable log level 1
 * FLECS_LOG_2 - Enable log level 2 and below
 * FLECS_LOG_3 - Enable log level 3 and below
 *
 * If no log level is defined and this is a debug build, FLECS_LOG_3 will
 * have been automatically defined.
 *
 * The provided level corresponds with the log level. If -1 is provided as
 * value, warnings are disabled. If -2 is provided, errors are disabled as well.
 *
 * @param level Desired tracing level.
 * @return Previous log level.
 */
FLECS_API
int ecs_log_set_level(
    int level);

/** Get current log level. 
 * 
 * @return Previous log level.
 */
FLECS_API
int ecs_log_get_level(void);

/** Enable/disable tracing with colors.
 * By default colors are enabled.
 *
 * @param enabled Whether to enable tracing with colors.
 * @return Previous color setting.
 */
FLECS_API
bool ecs_log_enable_colors(
    bool enabled);

/** Enable/disable logging timestamp.
 * By default timestamps are disabled. Note that enabling timestamps introduces
 * overhead as the logging code will need to obtain the current time.
 *
 * @param enabled Whether to enable tracing with timestamps.
 * @return Previous timestamp setting.
 */
FLECS_API
bool ecs_log_enable_timestamp(
    bool enabled);

/** Enable/disable logging time since last log.
 * By default deltatime is disabled. Note that enabling timestamps introduces
 * overhead as the logging code will need to obtain the current time.
 * 
 * When enabled, this logs the amount of time in seconds passed since the last
 * log, when this amount is non-zero. The format is a '+' character followed by
 * the number of seconds:
 * 
 *   +1 trace: log message
 *
 * @param enabled Whether to enable tracing with timestamps.
 * @return Previous timestamp setting.
 */
FLECS_API
bool ecs_log_enable_timedelta(
    bool enabled);

/** Get last logged error code.
 * Calling this operation resets the error code.
 *
 * @return Last error, 0 if none was logged since last call to last_error.
 */
FLECS_API
int ecs_log_last_error(void);


////////////////////////////////////////////////////////////////////////////////
//// Error codes
////////////////////////////////////////////////////////////////////////////////

#define ECS_INVALID_OPERATION (1)
#define ECS_INVALID_PARAMETER (2)
#define ECS_CONSTRAINT_VIOLATED (3)
#define ECS_OUT_OF_MEMORY (4)
#define ECS_OUT_OF_RANGE (5)
#define ECS_UNSUPPORTED (6)
#define ECS_INTERNAL_ERROR (7)
#define ECS_ALREADY_DEFINED (8)
#define ECS_MISSING_OS_API (9)
#define ECS_OPERATION_FAILED (10)
#define ECS_INVALID_CONVERSION (11)
#define ECS_ID_IN_USE (12)
#define ECS_CYCLE_DETECTED (13)
#define ECS_LEAK_DETECTED (14)
#define ECS_DOUBLE_FREE (15)

#define ECS_INCONSISTENT_NAME (20)
#define ECS_NAME_IN_USE (21)
#define ECS_NOT_A_COMPONENT (22)
#define ECS_INVALID_COMPONENT_SIZE (23)
#define ECS_INVALID_COMPONENT_ALIGNMENT (24)
#define ECS_COMPONENT_NOT_REGISTERED (25)
#define ECS_INCONSISTENT_COMPONENT_ID (26)
#define ECS_INCONSISTENT_COMPONENT_ACTION (27)
#define ECS_MODULE_UNDEFINED (28)
#define ECS_MISSING_SYMBOL (29)
#define ECS_ALREADY_IN_USE (30)

#define ECS_ACCESS_VIOLATION (40)
#define ECS_COLUMN_INDEX_OUT_OF_RANGE (41)
#define ECS_COLUMN_IS_NOT_SHARED (42)
#define ECS_COLUMN_IS_SHARED (43)
#define ECS_COLUMN_TYPE_MISMATCH (45)

#define ECS_INVALID_WHILE_READONLY (70)
#define ECS_LOCKED_STORAGE (71)
#define ECS_INVALID_FROM_WORKER (72)


////////////////////////////////////////////////////////////////////////////////
//// Used when logging with colors is enabled
////////////////////////////////////////////////////////////////////////////////

#define ECS_BLACK   "\033[1;30m"
#define ECS_RED     "\033[0;31m"
#define ECS_GREEN   "\033[0;32m"
#define ECS_YELLOW  "\033[0;33m"
#define ECS_BLUE    "\033[0;34m"
#define ECS_MAGENTA "\033[0;35m"
#define ECS_CYAN    "\033[0;36m"
#define ECS_WHITE   "\033[1;37m"
#define ECS_GREY    "\033[0;37m"
#define ECS_NORMAL  "\033[0;49m"
#define ECS_BOLD    "\033[1;49m"


/** @} */

#endif // FLECS_LOG_H


/* Handle addon dependencies that need declarations to be visible in header */
#ifdef FLECS_MONITOR
#ifndef FLECS_STATS
#define FLECS_STATS
#endif
#ifndef FLECS_SYSTEM
#define FLECS_SYSTEM
#endif
#ifndef FLECS_TIMER
#define FLECS_TIMER
#endif
#endif

#ifdef FLECS_REST
#define FLECS_HTTP
#endif

#ifdef FLECS_PLECS
#define FLECS_EXPR
#endif

#ifdef FLECS_APP
#ifdef FLECS_NO_APP
#error "FLECS_NO_APP failed: APP is required by other addons"
#endif
/**
 * @file addons/app.h
 * @brief App addon.
 *
 * The app addon is a wrapper around the application's main loop. Its main
 * purpose is to provide a hook to modules that need to take control of the
 * main loop, as is for example the case with native applications that use
 * emscripten with webGL.
 */

#ifdef FLECS_APP

#ifndef FLECS_PIPELINE
#define FLECS_PIPELINE
#endif

#ifndef FLECS_APP_H
#define FLECS_APP_H


/**
 * @defgroup c_addons_app App
 * @brief Optional addon for running the main application loop.
 * 
 * \ingroup c_addons
 * @{
 */

/** Callback type for init action. */
typedef int(*ecs_app_init_action_t)(
    ecs_world_t *world);

/** Used with ecs_app_run. */
typedef struct ecs_app_desc_t {
    ecs_ftime_t target_fps;   /**< Target FPS. */
    ecs_ftime_t delta_time;   /**< Frame time increment (0 for measured values) */
    int32_t threads;          /**< Number of threads. */
    int32_t frames;           /**< Number of frames to run (0 for infinite) */
    bool enable_rest;         /**< Enables ECS access over HTTP, necessary for explorer */
    bool enable_monitor;      /**< Periodically collect statistics */
    uint16_t port;            /**< HTTP port used by REST API */

    ecs_app_init_action_t init; /**< If set, function is ran before starting the
                                 * main loop. */

    void *ctx;                /**< Reserved for custom run/frame actions */
} ecs_app_desc_t;

/** Callback type for run action. */
typedef int(*ecs_app_run_action_t)(
    ecs_world_t *world, 
    ecs_app_desc_t *desc);

/** Callback type for frame action. */
typedef int(*ecs_app_frame_action_t)(
    ecs_world_t *world, 
    const ecs_app_desc_t *desc);

/** Run application.
 * This will run the application with the parameters specified in desc. After
 * the application quits (ecs_quit is called) the world will be cleaned up.
 * 
 * If a custom run action is set, it will be invoked by this operation. The
 * default run action calls the frame action in a loop until it returns a
 * non-zero value.
 * 
 * @param world The world.
 * @param desc Application parameters.
 */
FLECS_API
int ecs_app_run(
    ecs_world_t *world,
    ecs_app_desc_t *desc);

/** Default frame callback.
 * This operation will run a single frame. By default this operation will invoke
 * ecs_progress directly, unless a custom frame action is set.
 * 
 * @param world The world.
 * @param desc The desc struct passed to ecs_app_run.
 * @return value returned by ecs_progress
 */
FLECS_API
int ecs_app_run_frame(
    ecs_world_t *world,
    const ecs_app_desc_t *desc);

/** Set custom run action.
 * See ecs_app_run.
 * 
 * @param callback The run action.
 */
FLECS_API
int ecs_app_set_run_action(
    ecs_app_run_action_t callback);

/** Set custom frame action.
 * See ecs_app_run_frame.
 * 
 * @param callback The frame action.
 */
FLECS_API
int ecs_app_set_frame_action(
    ecs_app_frame_action_t callback);

/** @} */


#endif

#endif // FLECS_APP

#endif

#ifdef FLECS_HTTP
#ifdef FLECS_NO_HTTP
#error "FLECS_NO_HTTP failed: HTTP is required by other addons"
#endif
/**
 * @file addons/http.h
 * @brief HTTP addon.
 * 
 * Minimalistic HTTP server that can receive and reply to simple HTTP requests.
 * The main goal of this addon is to enable remotely connecting to a running
 * Flecs application (for example, with a web-based UI) and request/visualize
 * data from the ECS world.
 * 
 * Each server instance creates a single thread used for receiving requests.
 * Receiving requests are enqueued and handled when the application calls
 * ecs_http_server_dequeue. This increases latency of request handling vs.
 * responding directly in the receive thread, but is better suited for 
 * retrieving data from ECS applications, as requests can be processed by an ECS
 * system without having to lock the world.
 * 
 * This server is intended to be used in a development environment.
 */

#ifdef FLECS_HTTP

/**
 * @defgroup c_addons_http Http
 * @brief Simple HTTP server used for serving up REST API.
 * 
 * \ingroup c_addons
 * @{
 */

#if !defined(FLECS_OS_API_IMPL) && !defined(FLECS_NO_OS_API_IMPL)
#define FLECS_OS_API_IMPL
#endif

#ifndef FLECS_HTTP_H
#define FLECS_HTTP_H

/* Maximum number of headers in request */
#define ECS_HTTP_HEADER_COUNT_MAX (32)

/* Maximum number of query parameters in request */
#define ECS_HTTP_QUERY_PARAM_COUNT_MAX (32)


/** HTTP server */
typedef struct ecs_http_server_t ecs_http_server_t;

/** A connection manages communication with the remote host */
typedef struct {
    uint64_t id;
    ecs_http_server_t *server;

    char host[128];
    char port[16];
} ecs_http_connection_t;

/** Helper type used for headers & URL query parameters */
typedef struct {
    const char *key;
    const char *value;
} ecs_http_key_value_t;

/** Supported request methods */
typedef enum {
    EcsHttpGet,
    EcsHttpPost,
    EcsHttpPut,
    EcsHttpDelete,
    EcsHttpOptions,
    EcsHttpMethodUnsupported
} ecs_http_method_t;

/** A request */
typedef struct {
    uint64_t id;

    ecs_http_method_t method;
    char *path;
    char *body;
    ecs_http_key_value_t headers[ECS_HTTP_HEADER_COUNT_MAX];
    ecs_http_key_value_t params[ECS_HTTP_HEADER_COUNT_MAX];
    int32_t header_count;
    int32_t param_count;

    ecs_http_connection_t *conn;
} ecs_http_request_t;

/** A reply */
typedef struct {
    int code;                   /**< default = 200 */
    ecs_strbuf_t body;          /**< default = "" */
    const char* status;         /**< default = OK */
    const char* content_type;   /**< default = application/json */
    ecs_strbuf_t headers;       /**< default = "" */
} ecs_http_reply_t;

#define ECS_HTTP_REPLY_INIT \
    (ecs_http_reply_t){200, ECS_STRBUF_INIT, "OK", "application/json", ECS_STRBUF_INIT}

/* Global statistics. */
extern int64_t ecs_http_request_received_count;
extern int64_t ecs_http_request_invalid_count;
extern int64_t ecs_http_request_handled_ok_count;
extern int64_t ecs_http_request_handled_error_count;
extern int64_t ecs_http_request_not_handled_count;
extern int64_t ecs_http_request_preflight_count;
extern int64_t ecs_http_send_ok_count;
extern int64_t ecs_http_send_error_count;
extern int64_t ecs_http_busy_count;

/** Request callback.
 * Invoked for each valid request. The function should populate the reply and
 * return true. When the function returns false, the server will reply with a 
 * 404 (Not found) code. */
typedef bool (*ecs_http_reply_action_t)(
    const ecs_http_request_t* request, 
    ecs_http_reply_t *reply,
    void *ctx);

/** Used with ecs_http_server_init. */
typedef struct {
    ecs_http_reply_action_t callback; /**< Function called for each request  */
    void *ctx;                        /**< Passed to callback (optional) */
    uint16_t port;                    /**< HTTP port */
    const char *ipaddr;               /**< Interface to listen on (optional) */
    int32_t send_queue_wait_ms;       /**< Send queue wait time when empty */
} ecs_http_server_desc_t;

/** Create server. 
 * Use ecs_http_server_start to start receiving requests.
 * 
 * @param desc Server configuration parameters.
 * @return The new server, or NULL if creation failed.
 */
FLECS_API
ecs_http_server_t* ecs_http_server_init(
    const ecs_http_server_desc_t *desc);

/** Destroy server. 
 * This operation will stop the server if it was still running.
 * 
 * @param server The server to destroy.
 */
FLECS_API
void ecs_http_server_fini(
    ecs_http_server_t* server);

/** Start server. 
 * After this operation the server will be able to accept requests.
 * 
 * @param server The server to start.
 * @return Zero if successful, non-zero if failed.
 */
FLECS_API
int ecs_http_server_start(
    ecs_http_server_t* server);

/** Process server requests. 
 * This operation invokes the reply callback for each received request. No new
 * requests will be enqueued while processing requests.
 * 
 * @param server The server for which to process requests.
 */
FLECS_API
void ecs_http_server_dequeue(
    ecs_http_server_t* server,
    ecs_ftime_t delta_time);

/** Stop server. 
 * After this operation no new requests can be received.
 * 
 * @param server The server.
 */
FLECS_API
void ecs_http_server_stop(
    ecs_http_server_t* server);

/** Emulate a request.
 * The request string must be a valid HTTP request. A minimal example:
 *   GET /entity/flecs/core/World?label=true HTTP/1.1
 *
 * @param srv The server.
 * @param req The request.
 * @param len The length of the request (optional).
 * @return The reply.
 */
FLECS_API
int ecs_http_server_http_request(
    ecs_http_server_t* srv,
    const char *req,
    ecs_size_t len,
    ecs_http_reply_t *reply_out);

/** Convenience wrapper around ecs_http_server_request. */
FLECS_API
int ecs_http_server_request(
    ecs_http_server_t* srv,
    const char *method,
    const char *req,
    ecs_http_reply_t *reply_out);

/** Get context provided in ecs_http_server_desc_t */
FLECS_API
void* ecs_http_server_ctx(
    ecs_http_server_t* srv);

/** Find header in request. 
 * 
 * @param req The request.
 * @param name name of the header to find
 * @return The header value, or NULL if not found.
*/
FLECS_API
const char* ecs_http_get_header(
    const ecs_http_request_t* req,
    const char* name);

/** Find query parameter in request. 
 * 
 * @param req The request.
 * @param name The parameter name.
 * @return The decoded parameter value, or NULL if not found.
 */
FLECS_API
const char* ecs_http_get_param(
    const ecs_http_request_t* req,
    const char* name);


/** @} */

#endif // FLECS_HTTP_H

#endif // FLECS_HTTP

#endif

#ifdef FLECS_REST
#ifdef FLECS_NO_REST
#error "FLECS_NO_REST failed: REST is required by other addons"
#endif
/**
 * @file addons/rest.h
 * @brief REST API addon.
 *
 * A small REST API that uses the HTTP server and JSON serializer to provide
 * access to application data for remote applications.
 * 
 * A description of the API can be found in docs/RestApi.md
 */

#ifdef FLECS_REST

/**
 * @defgroup c_addons_rest Rest
 * @brief REST API for querying and mutating entities.
 * 
 * \ingroup c_addons
 * @{
 */

/* Used for the HTTP server */
#ifndef FLECS_HTTP
#define FLECS_HTTP
#endif

/* Used for building the JSON replies */
#ifndef FLECS_JSON
#define FLECS_JSON
#endif

/* Query engine used */
#ifndef FLECS_RULES
#define FLECS_RULES
#endif

/* For the REST system */
#ifndef FLECS_PIPELINE
#define FLECS_PIPELINE
#endif

#ifndef FLECS_REST_H
#define FLECS_REST_H


#define ECS_REST_DEFAULT_PORT (27750)

/** Component that instantiates the REST API */
FLECS_API extern const ecs_entity_t ecs_id(EcsRest);

typedef struct {
    uint16_t port;      /**< Port of server (optional, default = 27750) */
    char *ipaddr;       /**< Interface address (optional, default = 0.0.0.0) */
    void *impl;
} EcsRest;

/** Create HTTP server for REST API. 
 * This allows for the creation of a REST server that can be managed by the
 * application without using Flecs systems.
 * 
 * @param world The world.
 * @param desc The HTTP server descriptor.
 * @return The HTTP server, or NULL if failed.
 */
FLECS_API
ecs_http_server_t* ecs_rest_server_init(
    ecs_world_t *world,
    const ecs_http_server_desc_t *desc);

/** Cleanup REST HTTP server. 
 * The server must have been created with ecs_rest_server_init.
 */
FLECS_API
void ecs_rest_server_fini(
    ecs_http_server_t *srv);

/* Module import */
FLECS_API
void FlecsRestImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_TIMER
#ifdef FLECS_NO_TIMER
#error "FLECS_NO_TIMER failed: TIMER is required by other addons"
#endif
/**
 * @file addons/timer.h
 * @brief Timer module.
 *
 * Timers can be used to trigger actions at periodic or one-shot intervals. They
 * are typically used together with systems and pipelines.
 */

#ifdef FLECS_TIMER

/**
 * @defgroup c_addons_timer Timer
 * @brief Run systems at a time interval.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif

#ifndef FLECS_PIPELINE
#define FLECS_PIPELINE
#endif

#ifndef FLECS_TIMER_H
#define FLECS_TIMER_H


/** Component used for one shot/interval timer functionality */
typedef struct EcsTimer {
    ecs_ftime_t timeout;         /**< Timer timeout period */
    ecs_ftime_t time;            /**< Incrementing time value */
    ecs_ftime_t overshoot;       /**< Used to correct returned interval time */
    int32_t fired_count;         /**< Number of times ticked */
    bool active;                 /**< Is the timer active or not */
    bool single_shot;            /**< Is this a single shot timer */
} EcsTimer;

/** Apply a rate filter to a tick source */
typedef struct EcsRateFilter {
    ecs_entity_t src;            /**< Source of the rate filter */
    int32_t rate;                /**< Rate of the rate filter */
    int32_t tick_count;          /**< Number of times the rate filter ticked */
    ecs_ftime_t time_elapsed;    /**< Time elapsed since last tick */
} EcsRateFilter;


/** Set timer timeout.
 * This operation executes any systems associated with the timer after the
 * specified timeout value. If the entity contains an existing timer, the 
 * timeout value will be reset. The timer can be started and stopped with 
 * ecs_start_timer and ecs_stop_timer.
 *
 * The timer is synchronous, and is incremented each frame by delta_time.
 *
 * The tick_source entity will be a tick source after this operation. Tick
 * sources can be read by getting the EcsTickSource component. If the tick
 * source ticked this frame, the 'tick' member will be true. When the tick 
 * source is a system, the system will tick when the timer ticks.
 *
 * @param world The world.
 * @param tick_source The timer for which to set the timeout (0 to create one).
 * @param timeout The timeout value.
 * @return The timer entity.
 */
FLECS_API
ecs_entity_t ecs_set_timeout(
    ecs_world_t *world,
    ecs_entity_t tick_source,
    ecs_ftime_t timeout);

/** Get current timeout value for the specified timer.
 * This operation returns the value set by ecs_set_timeout. If no timer is
 * active for this entity, the operation returns 0.
 *
 * After the timeout expires the EcsTimer component is removed from the entity.
 * This means that if ecs_get_timeout is invoked after the timer is expired, the
 * operation will return 0.
 *
 * The timer is synchronous, and is incremented each frame by delta_time.
 *
 * The tick_source entity will be a tick source after this operation. Tick
 * sources can be read by getting the EcsTickSource component. If the tick
 * source ticked this frame, the 'tick' member will be true. When the tick 
 * source is a system, the system will tick when the timer ticks.
 *
 * @param world The world.
 * @param tick_source The timer.
 * @return The current timeout value, or 0 if no timer is active.
 */
FLECS_API
ecs_ftime_t ecs_get_timeout(
    const ecs_world_t *world,
    ecs_entity_t tick_source);

/** Set timer interval.
 * This operation will continuously invoke systems associated with the timer
 * after the interval period expires. If the entity contains an existing timer,
 * the interval value will be reset.
 *
 * The timer is synchronous, and is incremented each frame by delta_time.
 *
 * The tick_source entity will be a tick source after this operation. Tick
 * sources can be read by getting the EcsTickSource component. If the tick
 * source ticked this frame, the 'tick' member will be true. When the tick 
 * source is a system, the system will tick when the timer ticks. 
 *
 * @param world The world.
 * @param tick_source The timer for which to set the interval (0 to create one).
 * @param interval The interval value.
 * @return The timer entity.
 */
FLECS_API
ecs_entity_t ecs_set_interval(
    ecs_world_t *world,
    ecs_entity_t tick_source,
    ecs_ftime_t interval);   

/** Get current interval value for the specified timer.
 * This operation returns the value set by ecs_set_interval. If the entity is
 * not a timer, the operation will return 0.
 *
 * @param world The world.
 * @param tick_source The timer for which to set the interval.
 * @return The current interval value, or 0 if no timer is active.
 */
FLECS_API
ecs_ftime_t ecs_get_interval(
    const ecs_world_t *world,
    ecs_entity_t tick_source);

/** Start timer.
 * This operation resets the timer and starts it with the specified timeout.
 *
 * @param world The world.
 * @param tick_source The timer to start.
 */
FLECS_API
void ecs_start_timer(
    ecs_world_t *world,
    ecs_entity_t tick_source);

/** Stop timer
 * This operation stops a timer from triggering.
 *
 * @param world The world.
 * @param tick_source The timer to stop.
 */
FLECS_API
void ecs_stop_timer(
    ecs_world_t *world,
    ecs_entity_t tick_source);

/** Reset time value of timer to 0.
 * This operation resets the timer value to 0.
 * 
 * @param world The world.
 * @param tick_source The timer to reset.
 */
FLECS_API
void ecs_reset_timer(
    ecs_world_t *world,
    ecs_entity_t tick_source);

/** Enable randomizing initial time value of timers. 
 * Initializes timers with a random time value, which can improve scheduling as
 * systems/timers for the same interval don't all happen on the same tick.
 * 
 * @param world The world.
 */
FLECS_API
void ecs_randomize_timers(
    ecs_world_t *world);

/** Set rate filter.
 * This operation initializes a rate filter. Rate filters sample tick sources
 * and tick at a configurable multiple. A rate filter is a tick source itself,
 * which means that rate filters can be chained.
 *
 * Rate filters enable deterministic system execution which cannot be achieved
 * with interval timers alone. For example, if timer A has interval 2.0 and
 * timer B has interval 4.0, it is not guaranteed that B will tick at exactly
 * twice the multiple of A. This is partly due to the indeterministic nature of
 * timers, and partly due to floating point rounding errors. 
 *
 * Rate filters can be combined with timers (or other rate filters) to ensure 
 * that a system ticks at an exact multiple of a tick source (which can be
 * another system). If a rate filter is created with a rate of 1 it will tick
 * at the exact same time as its source.
 *
 * If no tick source is provided, the rate filter will use the frame tick as
 * source, which corresponds with the number of times ecs_progress is called.
 *
 * The tick_source entity will be a tick source after this operation. Tick
 * sources can be read by getting the EcsTickSource component. If the tick
 * source ticked this frame, the 'tick' member will be true. When the tick 
 * source is a system, the system will tick when the timer ticks.  
 *
 * @param world The world.
 * @param tick_source The rate filter entity (0 to create one).
 * @param rate The rate to apply.
 * @param source The tick source (0 to use frames)
 * @return The filter entity.
 */
FLECS_API
ecs_entity_t ecs_set_rate(
    ecs_world_t *world,
    ecs_entity_t tick_source,
    int32_t rate,
    ecs_entity_t source);

/** Assign tick source to system.
 * Systems can be their own tick source, which can be any of the tick sources
 * (one shot timers, interval times and rate filters). However, in some cases it
 * is must be guaranteed that different systems tick on the exact same frame.
 * 
 * This cannot be guaranteed by giving two systems the same interval/rate filter
 * as it is possible that one system is (for example) disabled, which would
 * cause the systems to go out of sync. To provide these guarantees, systems
 * must use the same tick source, which is what this operation enables.
 *
 * When two systems share the same tick source, it is guaranteed that they tick
 * in the same frame. The provided tick source can be any entity that is a tick
 * source, including another system. If the provided entity is not a tick source
 * the system will not be ran.
 *
 * To disassociate a tick source from a system, use 0 for the tick_source 
 * parameter.
 *
 * @param world The world.
 * @param system The system to associate with the timer.
 * @param tick_source The tick source to associate with the system.
 */ 
FLECS_API
void ecs_set_tick_source(
    ecs_world_t *world,
    ecs_entity_t system,
    ecs_entity_t tick_source);


////////////////////////////////////////////////////////////////////////////////
//// Module
////////////////////////////////////////////////////////////////////////////////

FLECS_API
void FlecsTimerImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_PIPELINE
#ifdef FLECS_NO_PIPELINE
#error "FLECS_NO_PIPELINE failed: PIPELINE is required by other addons"
#endif
/**
 * @file addons/pipeline.h
 * @brief Pipeline module.
 *
 * The pipeline module provides support for running systems automatically and
 * on multiple threads. A pipeline is a collection of tags that can be added to
 * systems. When ran, a pipeline will query for all systems that have the tags
 * that belong to a pipeline, and run them.
 *
 * The module defines a number of builtin tags (EcsPreUpdate, EcsOnUpdate, 
 * EcsPostUpdate etc.) that are registered with the builtin pipeline. The 
 * builtin pipeline is ran by default when calling ecs_progress(). An 
 * application can set a custom pipeline with the ecs_set_pipeline function.
 */

#ifdef FLECS_PIPELINE

/**
 * @defgroup c_addons_pipeline Pipeline
 * @brief Pipelines order and schedule systems for execution.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif

#ifndef FLECS_SYSTEM
#define FLECS_SYSTEM
#endif

#if !defined(FLECS_OS_API_IMPL) && !defined(FLECS_NO_OS_API_IMPL)
#define FLECS_OS_API_IMPL
#endif

#ifndef FLECS_PIPELINE_H
#define FLECS_PIPELINE_H


#ifndef FLECS_LEGACY

#define ECS_PIPELINE_DEFINE(world, id_, ...) \
    { \
        ecs_pipeline_desc_t desc = {0}; \
        ecs_entity_desc_t edesc = {0}; \
        edesc.id = id_;\
        edesc.name = #id_;\
        desc.entity = ecs_entity_init(world, &edesc);\
        desc.query.filter.expr = #__VA_ARGS__; \
        id_ = ecs_pipeline_init(world, &desc); \
        ecs_id(id_) = id_;\
    } \
    ecs_assert(id_ != 0, ECS_INVALID_PARAMETER, NULL);

#define ECS_PIPELINE(world, id, ...) \
    ecs_entity_t id = 0, ecs_id(id) = 0; ECS_PIPELINE_DEFINE(world, id, __VA_ARGS__);\
    (void)id;\
    (void)ecs_id(id);

#define ecs_pipeline(world, ...)\
    ecs_pipeline_init(world, &(ecs_pipeline_desc_t) __VA_ARGS__ )

#endif

/* Pipeline descriptor (used with ecs_pipeline_init) */
typedef struct ecs_pipeline_desc_t {
    /* Existing entity to associate with pipeline (optional) */
    ecs_entity_t entity;
    
    /* Query descriptor. The first term of the query must match the EcsSystem
     * component. */
    ecs_query_desc_t query;
} ecs_pipeline_desc_t;

/** Create a custom pipeline.
 */
FLECS_API
ecs_entity_t ecs_pipeline_init(
    ecs_world_t *world,
    const ecs_pipeline_desc_t *desc);

/** Set a custom pipeline.
 * This operation sets the pipeline to run when ecs_progress is invoked.
 *
 * @param world The world.
 * @param pipeline The pipeline to set.
 */
FLECS_API
void ecs_set_pipeline(
    ecs_world_t *world,
    ecs_entity_t pipeline);       

/** Get the current pipeline.
 * This operation gets the current pipeline.
 *
 * @param world The world.
 * @return The current pipeline.
 */
FLECS_API
ecs_entity_t ecs_get_pipeline(
    const ecs_world_t *world);  

/** Progress a world.
 * This operation progresses the world by running all systems that are both
 * enabled and periodic on their matching entities.
 *
 * An application can pass a delta_time into the function, which is the time
 * passed since the last frame. This value is passed to systems so they can
 * update entity values proportional to the elapsed time since their last
 * invocation.
 *
 * When an application passes 0 to delta_time, ecs_progress will automatically
 * measure the time passed since the last frame. If an application does not uses
 * time management, it should pass a non-zero value for delta_time (1.0 is
 * recommended). That way, no time will be wasted measuring the time.
 *
 * @param world The world to progress.
 * @param delta_time The time passed since the last frame.
 * @return false if ecs_quit has been called, true otherwise.
 */
FLECS_API
bool ecs_progress(
    ecs_world_t *world,
    ecs_ftime_t delta_time);   

/** Set time scale.
 * Increase or decrease simulation speed by the provided multiplier.
 *
 * @param world The world.
 * @param scale The scale to apply (default = 1).
 */
FLECS_API 
void ecs_set_time_scale(
    ecs_world_t *world,
    ecs_ftime_t scale);

/** Reset world clock.
 * Reset the clock that keeps track of the total time passed in the simulation.
 *
 * @param world The world.
 */
FLECS_API
void ecs_reset_clock(
    ecs_world_t *world);

/** Run pipeline.
 * This will run all systems in the provided pipeline. This operation may be
 * invoked from multiple threads, and only when staging is disabled, as the
 * pipeline manages staging and, if necessary, synchronization between threads.
 *
 * If 0 is provided for the pipeline id, the default pipeline will be ran (this
 * is either the builtin pipeline or the pipeline set with set_pipeline()). 
 *
 * When using progress() this operation will be invoked automatically for the
 * default pipeline (either the builtin pipeline or the pipeline set with 
 * set_pipeline()). An application may run additional pipelines.
 *
 * @param world The world.
 * @param pipeline The pipeline to run.
 */
FLECS_API 
void ecs_run_pipeline(
    ecs_world_t *world,
    ecs_entity_t pipeline,
    ecs_ftime_t delta_time);


////////////////////////////////////////////////////////////////////////////////
//// Threading
////////////////////////////////////////////////////////////////////////////////

/** Set number of worker threads.
 * Setting this value to a value higher than 1 will start as many threads and
 * will cause systems to evenly distribute matched entities across threads. The
 * operation may be called multiple times to reconfigure the number of threads
 * used, but never while running a system / pipeline. 
 * Calling ecs_set_threads will also end the use of task threads setup with 
 * ecs_set_task_threads and vice-versa */
FLECS_API
void ecs_set_threads(
    ecs_world_t *world,
    int32_t threads);

/** Set number of worker task threads.
 * ecs_set_task_threads is similar to ecs_set_threads, except threads are treated
 * as short-lived tasks and will be created and joined around each update of the world. 
 * Creation and joining of these tasks will use the os_api_t tasks APIs rather than the
 * the standard thread API functions, although they may be the same if desired.
 * This function is useful for multithreading world updates using an external
 * asynchronous job system rather than long running threads by providing the APIs
 * to create tasks for your job system and then wait on their conclusion. 
 * The operation may be called multiple times to reconfigure the number of task threads
 * used, but never while running a system / pipeline. 
 * Calling ecs_set_task_threads will also end the use of threads setup with 
 * ecs_set_threads and vice-versa */

FLECS_API
void ecs_set_task_threads(
    ecs_world_t *world,
    int32_t task_threads);

/** Returns true if task thread use have been requested. */
FLECS_API
bool ecs_using_task_threads(
    ecs_world_t *world);

////////////////////////////////////////////////////////////////////////////////
//// Module
////////////////////////////////////////////////////////////////////////////////

FLECS_API
void FlecsPipelineImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_SYSTEM
#ifdef FLECS_NO_SYSTEM
#error "FLECS_NO_SYSTEM failed: SYSTEM is required by other addons"
#endif
/**
 * @file addons/system.h
 * @brief System module.
 *
 * The system module allows for creating and running systems. A system is a
 * query in combination with a callback function. In addition systems have
 * support for time management and can be monitored by the stats addon.
 */

#ifdef FLECS_SYSTEM

/**
 * @defgroup c_addons_system System
 * @brief Systems are a query + function that can be ran manually or by a pipeline.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif

#ifndef FLECS_SYSTEM_H
#define FLECS_SYSTEM_H


/** Component used to provide a tick source to systems */
typedef struct EcsTickSource {
    bool tick;                 /**< True if providing tick */
    ecs_ftime_t time_elapsed;  /**< Time elapsed since last tick */
} EcsTickSource;

/** Use with ecs_system_init */
typedef struct ecs_system_desc_t {
    int32_t _canary;

    /** Existing entity to associate with system (optional) */
    ecs_entity_t entity;

    /** System query parameters */
    ecs_query_desc_t query;

    /** Callback that is invoked when a system is ran. 
     * When left to NULL, the default system runner is used, which calls the 
     * "callback" action for each result returned from the system's query. 
     * 
     * It should not be assumed that the input iterator can always be iterated
     * with ecs_query_next. When a system is multithreaded and/or paged, the
     * iterator can be either a worker or paged iterator. Future use cases may
     * introduce additional inputs for a system, such as rules and filters. The
     * correct function to use for iteration is ecs_iter_next.
     * 
     * An implementation can test whether the iterator is a query iterator by
     * testing whether the it->next value is equal to ecs_query_next. */
    ecs_run_action_t run;

    /** Callback that is ran for each result returned by the system's query. This
     * means that this callback can be invoked multiple times per system per
     * frame, typically once for each matching table. */
    ecs_iter_action_t callback;

    /** Context to be passed to callback (as ecs_iter_t::param) */
    void *ctx;

    /** Binding context, for when system is implemented in other language */
    void *binding_ctx;

    /** Functions that are invoked during system cleanup to free context data.
     * When set, functions are called unconditionally, even when the ctx
     * pointers are NULL. */
    ecs_ctx_free_t ctx_free;
    ecs_ctx_free_t binding_ctx_free;

    /** Interval in seconds at which the system should run */
    ecs_ftime_t interval;

    /** Rate at which the system should run */
    int32_t rate;

    /** External tick source that determines when system ticks */
    ecs_entity_t tick_source;

    /** If true, system will be ran on multiple threads */
    bool multi_threaded;

    /** If true, system will have access to the actual world. Cannot be true at the
     * same time as multi_threaded. */
    bool no_readonly;
} ecs_system_desc_t;

/** Create a system */
FLECS_API
ecs_entity_t ecs_system_init(
    ecs_world_t *world,
    const ecs_system_desc_t *desc);

#ifndef FLECS_LEGACY

/** Forward declare a system. */
#define ECS_SYSTEM_DECLARE(id) ecs_entity_t ecs_id(id)

/** Define a forward declared system.
 * 
 * Example:
 *   ECS_SYSTEM_DEFINE(world, Move, EcsOnUpdate, Position, Velocity);
 */
#define ECS_SYSTEM_DEFINE(world, id_, phase, ...) \
    { \
        ecs_system_desc_t desc = {0}; \
        ecs_entity_desc_t edesc = {0}; \
        edesc.id = ecs_id(id_);\
        edesc.name = #id_;\
        edesc.add[0] = ((phase) ? ecs_pair(EcsDependsOn, (phase)) : 0); \
        edesc.add[1] = (phase); \
        desc.entity = ecs_entity_init(world, &edesc);\
        desc.query.filter.expr = #__VA_ARGS__; \
        desc.callback = id_; \
        ecs_id(id_) = ecs_system_init(world, &desc); \
    } \
    ecs_assert(ecs_id(id_) != 0, ECS_INVALID_PARAMETER, NULL)

/** Declare & define a system.
 * 
 * Example:
 *   ECS_SYSTEM(world, Move, EcsOnUpdate, Position, Velocity);
 */
#define ECS_SYSTEM(world, id, phase, ...) \
    ecs_entity_t ecs_id(id) = 0; ECS_SYSTEM_DEFINE(world, id, phase, __VA_ARGS__);\
    ecs_entity_t id = ecs_id(id);\
    (void)ecs_id(id);\
    (void)id

/** Shorthand for creating a system with ecs_system_init.
 *
 * Example:
 *   ecs_system(world, {
 *     .entity = ecs_entity(world, {
 *       .name = "MyEntity",
 *       .add = { ecs_dependson(EcsOnUpdate) }
 *     }),
 *     .query.filter.terms = {
 *       { ecs_id(Position) },
 *       { ecs_id(Velocity) }
 *     },
 *     .callback = Move
 *   });
 */
#define ecs_system(world, ...)\
    ecs_system_init(world, &(ecs_system_desc_t) __VA_ARGS__ )

#endif

/** Run a specific system manually.
 * This operation runs a single system manually. It is an efficient way to
 * invoke logic on a set of entities, as manual systems are only matched to
 * tables at creation time or after creation time, when a new table is created.
 *
 * Manual systems are useful to evaluate lists of prematched entities at
 * application defined times. Because none of the matching logic is evaluated
 * before the system is invoked, manual systems are much more efficient than
 * manually obtaining a list of entities and retrieving their components.
 *
 * An application may pass custom data to a system through the param parameter.
 * This data can be accessed by the system through the param member in the
 * ecs_iter_t value that is passed to the system callback.
 *
 * Any system may interrupt execution by setting the interrupted_by member in
 * the ecs_iter_t value. This is particularly useful for manual systems, where
 * the value of interrupted_by is returned by this operation. This, in
 * combination with the param argument lets applications use manual systems
 * to lookup entities: once the entity has been found its handle is passed to
 * interrupted_by, which is then subsequently returned.
 *
 * @param world The world.
 * @param system The system to run.
 * @param delta_time The time passed since the last system invocation.
 * @param param A user-defined parameter to pass to the system.
 * @return handle to last evaluated entity if system was interrupted.
 */
FLECS_API
ecs_entity_t ecs_run(
    ecs_world_t *world,
    ecs_entity_t system,
    ecs_ftime_t delta_time,
    void *param);

/** Same as ecs_run, but subdivides entities across number of provided stages.
 * 
 * @param world The world.
 * @param system The system to run.
 * @param stage_current The id of the current stage.
 * @param stage_count The total number of stages.
 * @param delta_time The time passed since the last system invocation.
 * @param param A user-defined parameter to pass to the system.
 * @return handle to last evaluated entity if system was interrupted.
 */
FLECS_API
ecs_entity_t ecs_run_worker(
    ecs_world_t *world,
    ecs_entity_t system,
    int32_t stage_current,
    int32_t stage_count,
    ecs_ftime_t delta_time,
    void *param);

/** Run system with offset/limit and type filter.
 * This operation is the same as ecs_run, but filters the entities that will be
 * iterated by the system.
 * 
 * Entities can be filtered in two ways. Offset and limit control the range of
 * entities that is iterated over. The range is applied to all entities matched
 * with the system, thus may cover multiple archetypes.
 * 
 * The type filter controls which entity types the system will evaluate. Only
 * types that contain all components in the type filter will be iterated over. A
 * type filter is only evaluated once per table, which makes filtering cheap if
 * the number of entities is large and the number of tables is small, but not as
 * cheap as filtering in the system signature.
 * 
 * @param world The world.
 * @param system The system to invoke.
 * @param delta_time The time passed since the last system invocation.
 * @param param A user-defined parameter to pass to the system.
 * @return handle to last evaluated entity if system was interrupted.
 */
FLECS_API
ecs_entity_t ecs_run_w_filter(
    ecs_world_t *world,
    ecs_entity_t system,
    ecs_ftime_t delta_time,
    int32_t offset,
    int32_t limit,
    void *param);

/** Get the query object for a system.
 * Systems use queries under the hood. This enables an application to get access
 * to the underlying query object of a system. This can be useful when, for 
 * example, an application needs to enable sorting for a system.
 *
 * @param world The world.
 * @param system The system from which to obtain the query.
 * @return The query.
 */
FLECS_API
ecs_query_t* ecs_system_get_query(
    const ecs_world_t *world,
    ecs_entity_t system);

/** Get system context.
 * This operation returns the context pointer set for the system. If
 * the provided entity is not a system, the function will return NULL.
 *
 * @param world The world.
 * @param system The system from which to obtain the context.
 * @return The context.
 */
FLECS_API
void* ecs_system_get_ctx(
    const ecs_world_t *world,
    ecs_entity_t system);

/** Get system binding context.
 * The binding context is a context typically used to attach any language 
 * binding specific data that is needed when invoking a callback that is 
 * implemented in another language.
 *
 * @param world The world.
 * @param system The system from which to obtain the context.
 * @return The context.
 */
FLECS_API
void* ecs_system_get_binding_ctx(
    const ecs_world_t *world,
    ecs_entity_t system);

FLECS_API
void FlecsSystemImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_STATS
#ifdef FLECS_NO_STATS
#error "FLECS_NO_STATS failed: STATS is required by other addons"
#endif
/**
 * @file addons/stats.h
 * @brief Statistics addon.
 *
 * The statistics addon enables an application to obtain detailed metrics about
 * the storage, systems and operations of a world.
 */

#ifdef FLECS_STATS

/**
 * @defgroup c_addons_stats Stats
 * @brief Collection of statistics for world, queries, systems and pipelines.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_STATS_H
#define FLECS_STATS_H


#define ECS_STAT_WINDOW (60)

/** Simple value that indicates current state */
typedef struct ecs_gauge_t {
    ecs_float_t avg[ECS_STAT_WINDOW];
    ecs_float_t min[ECS_STAT_WINDOW];
    ecs_float_t max[ECS_STAT_WINDOW];
} ecs_gauge_t;

/** Monotonically increasing counter */
typedef struct ecs_counter_t {
    ecs_gauge_t rate;                     /**< Keep track of deltas too */
    double value[ECS_STAT_WINDOW];
} ecs_counter_t;

/** Make all metrics the same size, so we can iterate over fields */
typedef union ecs_metric_t {
    ecs_gauge_t gauge;
    ecs_counter_t counter;
} ecs_metric_t;

typedef struct ecs_world_stats_t {
    int64_t first_;

    /* Entities */
    struct {
        ecs_metric_t count;               /**< Number of entities */
        ecs_metric_t not_alive_count;     /**< Number of not alive (recyclable) entity ids */
    } entities;

    /* Component ids */
    struct {
        ecs_metric_t tag_count;           /**< Number of tag ids (ids without data) */
        ecs_metric_t component_count;     /**< Number of components ids (ids with data) */
        ecs_metric_t pair_count;          /**< Number of pair ids */
        ecs_metric_t type_count;          /**< Number of registered types */
        ecs_metric_t create_count;        /**< Number of times id has been created */
        ecs_metric_t delete_count;        /**< Number of times id has been deleted */
    } components;

    /* Tables */
    struct {
        ecs_metric_t count;                /**< Number of tables */
        ecs_metric_t empty_count;          /**< Number of empty tables */
        ecs_metric_t create_count;         /**< Number of times table has been created */
        ecs_metric_t delete_count;         /**< Number of times table has been deleted */
    } tables;

    /* Queries & events */
    struct {
        ecs_metric_t query_count;          /**< Number of queries */
        ecs_metric_t observer_count;       /**< Number of observers */
        ecs_metric_t system_count;         /**< Number of systems */
    } queries;

    /* Commands */
    struct {
        ecs_metric_t add_count;
        ecs_metric_t remove_count;
        ecs_metric_t delete_count;
        ecs_metric_t clear_count;
        ecs_metric_t set_count;
        ecs_metric_t get_mut_count;
        ecs_metric_t modified_count;
        ecs_metric_t other_count;
        ecs_metric_t discard_count;
        ecs_metric_t batched_entity_count;
        ecs_metric_t batched_count;
    } commands;

    /* Frame data */
    struct {
        ecs_metric_t frame_count;          /**< Number of frames processed. */
        ecs_metric_t merge_count;          /**< Number of merges executed. */
        ecs_metric_t rematch_count;        /**< Number of query rematches */
        ecs_metric_t pipeline_build_count; /**< Number of system pipeline rebuilds (occurs when an inactive system becomes active). */
        ecs_metric_t systems_ran;          /**< Number of systems ran. */
        ecs_metric_t observers_ran;        /**< Number of times an observer was invoked. */
        ecs_metric_t event_emit_count;     /**< Number of events emitted */
    } frame;

    /* Timing */
    struct {
        ecs_metric_t world_time_raw;       /**< Actual time passed since simulation start (first time progress() is called) */
        ecs_metric_t world_time;           /**< Simulation time passed since simulation start. Takes into account time scaling */
        ecs_metric_t frame_time;           /**< Time spent processing a frame. Smaller than world_time_total when load is not 100% */
        ecs_metric_t system_time;          /**< Time spent on running systems. */
        ecs_metric_t emit_time;            /**< Time spent on notifying observers. */
        ecs_metric_t merge_time;           /**< Time spent on merging commands. */
        ecs_metric_t rematch_time;         /**< Time spent on rematching. */
        ecs_metric_t fps;                  /**< Frames per second. */
        ecs_metric_t delta_time;           /**< Delta_time. */
    } performance;

    struct {
        /* Memory allocation data */
        ecs_metric_t alloc_count;          /**< Allocs per frame */
        ecs_metric_t realloc_count;        /**< Reallocs per frame */
        ecs_metric_t free_count;           /**< Frees per frame */
        ecs_metric_t outstanding_alloc_count; /**< Difference between allocs & frees */

        /* Memory allocator data */
        ecs_metric_t block_alloc_count;    /**< Block allocations per frame */
        ecs_metric_t block_free_count;     /**< Block frees per frame */
        ecs_metric_t block_outstanding_alloc_count; /**< Difference between allocs & frees */
        ecs_metric_t stack_alloc_count;    /**< Page allocations per frame */
        ecs_metric_t stack_free_count;     /**< Page frees per frame */
        ecs_metric_t stack_outstanding_alloc_count; /**< Difference between allocs & frees */
    } memory;

    /* HTTP statistics */
    struct {
        ecs_metric_t request_received_count;
        ecs_metric_t request_invalid_count;
        ecs_metric_t request_handled_ok_count;
        ecs_metric_t request_handled_error_count;
        ecs_metric_t request_not_handled_count;
        ecs_metric_t request_preflight_count;
        ecs_metric_t send_ok_count;
        ecs_metric_t send_error_count;
        ecs_metric_t busy_count;
    } http;

    int64_t last_;

    /** Current position in ringbuffer */
    int32_t t;
} ecs_world_stats_t;

/** Statistics for a single query (use ecs_query_stats_get) */
typedef struct ecs_query_stats_t {
    int64_t first_;
    ecs_metric_t matched_table_count;       /**< Matched non-empty tables */    
    ecs_metric_t matched_empty_table_count; /**< Matched empty tables */
    ecs_metric_t matched_entity_count;      /**< Number of matched entities */
    int64_t last_;

    /** Current position in ringbuffer */
    int32_t t; 
} ecs_query_stats_t;

/** Statistics for a single system (use ecs_system_stats_get) */
typedef struct ecs_system_stats_t {
    int64_t first_;
    ecs_metric_t time_spent;       /**< Time spent processing a system */
    ecs_metric_t invoke_count;     /**< Number of times system is invoked */
    int64_t last_;

    bool task;                     /**< Is system a task */

    ecs_query_stats_t query;
} ecs_system_stats_t;

/** Statistics for sync point */
typedef struct ecs_sync_stats_t {
    int64_t first_;
    ecs_metric_t time_spent;
    ecs_metric_t commands_enqueued;
    int64_t last_;

    int32_t system_count;
    bool multi_threaded;
    bool no_readonly;
} ecs_sync_stats_t;

/** Statistics for all systems in a pipeline. */
typedef struct ecs_pipeline_stats_t {
    /* Allow for initializing struct with {0} */
    int8_t canary_;

    /** Vector with system ids of all systems in the pipeline. The systems are
     * stored in the order they are executed. Merges are represented by a 0. */
    ecs_vec_t systems;
    
    /** Vector with sync point stats */
    ecs_vec_t sync_points;

    /** Map with system statistics. For each system in the systems vector, an
     * entry in the map exists of type ecs_system_stats_t. */
    ecs_map_t system_stats;

    /** Current position in ringbuffer */
    int32_t t;

    int32_t system_count;        /**< Number of systems in pipeline */
    int32_t active_system_count; /**< Number of active systems in pipeline */
    int32_t rebuild_count;       /**< Number of times pipeline has rebuilt */
} ecs_pipeline_stats_t;

/** Get world statistics.
 *
 * @param world The world.
 * @param stats Out parameter for statistics.
 */
FLECS_API 
void ecs_world_stats_get(
    const ecs_world_t *world,
    ecs_world_stats_t *stats);

/** Reduce source measurement window into single destination measurement. */
FLECS_API 
void ecs_world_stats_reduce(
    ecs_world_stats_t *dst,
    const ecs_world_stats_t *src);

/** Reduce last measurement into previous measurement, restore old value. */
FLECS_API
void ecs_world_stats_reduce_last(
    ecs_world_stats_t *stats,
    const ecs_world_stats_t *old,
    int32_t count);

/** Repeat last measurement. */
FLECS_API
void ecs_world_stats_repeat_last(
    ecs_world_stats_t *stats);

/** Copy last measurement from source to destination. */
FLECS_API
void ecs_world_stats_copy_last(
    ecs_world_stats_t *dst,
    const ecs_world_stats_t *src);

FLECS_API 
void ecs_world_stats_log(
    const ecs_world_t *world,
    const ecs_world_stats_t *stats);

/** Get query statistics.
 * Obtain statistics for the provided query.
 *
 * @param world The world.
 * @param query The query.
 * @param stats Out parameter for statistics.
 */
FLECS_API 
void ecs_query_stats_get(
    const ecs_world_t *world,
    const ecs_query_t *query,
    ecs_query_stats_t *stats);

/** Reduce source measurement window into single destination measurement. */
FLECS_API 
void ecs_query_stats_reduce(
    ecs_query_stats_t *dst,
    const ecs_query_stats_t *src);

/** Reduce last measurement into previous measurement, restore old value. */
FLECS_API
void ecs_query_stats_reduce_last(
    ecs_query_stats_t *stats,
    const ecs_query_stats_t *old,
    int32_t count);

/** Repeat last measurement. */
FLECS_API
void ecs_query_stats_repeat_last(
    ecs_query_stats_t *stats);

/** Copy last measurement from source to destination. */
FLECS_API
void ecs_query_stats_copy_last(
    ecs_query_stats_t *dst,
    const ecs_query_stats_t *src);

#ifdef FLECS_SYSTEM
/** Get system statistics.
 * Obtain statistics for the provided system.
 *
 * @param world The world.
 * @param system The system.
 * @param stats Out parameter for statistics.
 * @return true if success, false if not a system.
 */
FLECS_API 
bool ecs_system_stats_get(
    const ecs_world_t *world,
    ecs_entity_t system,
    ecs_system_stats_t *stats);

/** Reduce source measurement window into single destination measurement */
FLECS_API 
void ecs_system_stats_reduce(
    ecs_system_stats_t *dst,
    const ecs_system_stats_t *src);

/** Reduce last measurement into previous measurement, restore old value. */
FLECS_API
void ecs_system_stats_reduce_last(
    ecs_system_stats_t *stats,
    const ecs_system_stats_t *old,
    int32_t count);

/** Repeat last measurement. */
FLECS_API
void ecs_system_stats_repeat_last(
    ecs_system_stats_t *stats);

/** Copy last measurement from source to destination. */
FLECS_API
void ecs_system_stats_copy_last(
    ecs_system_stats_t *dst,
    const ecs_system_stats_t *src);
#endif

#ifdef FLECS_PIPELINE
/** Get pipeline statistics.
 * Obtain statistics for the provided pipeline.
 *
 * @param world The world.
 * @param pipeline The pipeline.
 * @param stats Out parameter for statistics.
 * @return true if success, false if not a pipeline.
 */
FLECS_API 
bool ecs_pipeline_stats_get(
    ecs_world_t *world,
    ecs_entity_t pipeline,
    ecs_pipeline_stats_t *stats);

/** Free pipeline stats.
 * 
 * @param stats The stats to free.
 */
FLECS_API
void ecs_pipeline_stats_fini(
    ecs_pipeline_stats_t *stats);

/** Reduce source measurement window into single destination measurement */
FLECS_API 
void ecs_pipeline_stats_reduce(
    ecs_pipeline_stats_t *dst,
    const ecs_pipeline_stats_t *src);

/** Reduce last measurement into previous measurement, restore old value. */
FLECS_API
void ecs_pipeline_stats_reduce_last(
    ecs_pipeline_stats_t *stats,
    const ecs_pipeline_stats_t *old,
    int32_t count);

/** Repeat last measurement. */
FLECS_API
void ecs_pipeline_stats_repeat_last(
    ecs_pipeline_stats_t *stats);

/** Copy last measurement to destination.
 * This operation copies the last measurement into the destination. It does not
 * modify the cursor.
 * 
 * @param dst The metrics.
 * @param src The metrics to copy.
 */
FLECS_API
void ecs_pipeline_stats_copy_last(
    ecs_pipeline_stats_t *dst,
    const ecs_pipeline_stats_t *src);

#endif

/** Reduce all measurements from a window into a single measurement. */
FLECS_API 
void ecs_metric_reduce(
    ecs_metric_t *dst,
    const ecs_metric_t *src,
    int32_t t_dst,
    int32_t t_src);

/** Reduce last measurement into previous measurement */
FLECS_API
void ecs_metric_reduce_last(
    ecs_metric_t *m,
    int32_t t,
    int32_t count);

/** Copy measurement */
FLECS_API
void ecs_metric_copy(
    ecs_metric_t *m,
    int32_t dst,
    int32_t src);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_METRICS
#ifdef FLECS_NO_METRICS
#error "FLECS_NO_METRICS failed: METRICS is required by other addons"
#endif
/**
 * @file addons/metrics.h
 * @brief Metrics module.
 *
 * The metrics module extracts metrics from components and makes them available
 * through a unified component interface.
 */

#ifdef FLECS_METRICS

/**
 * @defgroup c_addons_metrics Metrics
 * @brief Collect user-defined metrics from ECS data.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_METRICS_H
#define FLECS_METRICS_H

#ifndef FLECS_META
#define FLECS_META
#endif

#ifndef FLECS_UNITS
#define FLECS_UNITS
#endif

#ifndef FLECS_PIPELINE
#define FLECS_PIPELINE
#endif


FLECS_API extern ECS_COMPONENT_DECLARE(FlecsMetrics);

/** Tag added to metrics, and used as first element of metric kind pair */
FLECS_API extern ECS_TAG_DECLARE(EcsMetric);

/** Metric that has monotonically increasing value */
FLECS_API extern ECS_TAG_DECLARE(EcsCounter);

/** Counter metric that is auto-incremented by source value */
FLECS_API extern ECS_TAG_DECLARE(EcsCounterIncrement);

/** Counter metric that counts the number of entities with an id */
FLECS_API extern ECS_TAG_DECLARE(EcsCounterId);

/** Metric that represents current value */
FLECS_API extern ECS_TAG_DECLARE(EcsGauge);

/** Tag added to metric instances */
FLECS_API extern ECS_TAG_DECLARE(EcsMetricInstance);

/** Component with metric instance value */
FLECS_API extern ECS_COMPONENT_DECLARE(EcsMetricValue);

/** Component with entity source of metric instance */
FLECS_API extern ECS_COMPONENT_DECLARE(EcsMetricSource);

typedef struct EcsMetricValue {
    double value;
} EcsMetricValue;

typedef struct EcsMetricSource {
    ecs_entity_t entity;
} EcsMetricSource;

typedef struct ecs_metric_desc_t {
    int32_t _canary;

    /** Entity associated with metric */
    ecs_entity_t entity;
    
    /** Entity associated with member that stores metric value. Must not be set
     * at the same time as id. Cannot be combined with EcsCounterId. */
    ecs_entity_t member;

    /* Member dot expression. Can be used instead of member and supports nested
     * members. Must be set together with id and should not be set at the same 
     * time as member. */
    const char *dotmember;

    /** Tracks whether entities have the specified component id. Must not be set
     * at the same time as member. */
    ecs_id_t id;

    /** If id is a (R, *) wildcard and relationship R has the OneOf property,
     * setting this value to true will track individual targets. 
     * If the kind is EcsCountId and the id is a (R, *) wildcard, this value
     * will create a metric per target. */
    bool targets;

    /** Must be EcsGauge, EcsCounter, EcsCounterIncrement or EcsCounterId */
    ecs_entity_t kind;

    /** Description of metric. Will only be set if FLECS_DOC addon is enabled */
    const char *brief;
} ecs_metric_desc_t;

/** Create a new metric.
 * Metrics are entities that store values measured from a range of different
 * properties in the ECS storage. Metrics provide a single unified interface to
 * discovering and reading these values, which can be useful for monitoring
 * utilities, or for debugging.
 * 
 * Examples of properties that can be measured by metrics are:
 *  - Component member values
 *  - How long an entity has had a specific component
 *  - How long an entity has had a specific target for a relationship
 *  - How many entities have a specific component
 * 
 * Metrics can either be created as a "gauge" or "counter". A gauge is a metric
 * that represents the value of something at a specific point in time, for
 * example "velocity". A counter metric represents a value that is monotonically
 * increasing, for example "miles driven".
 * 
 * There are three different kinds of counter metric kinds:
 * - EcsCounter
 *   When combined with a member, this will store the actual value of the member
 *   in the metric. This is useful for values that are already counters, such as
 *   a MilesDriven component.
 *   This kind creates a metric per entity that has the member/id.
 * 
 * - EcsCounterIncrement
 *   When combined with a member, this will increment the value of the metric by
 *   the value of the member * delta_time. This is useful for values that are
 *   not counters, such as a Velocity component.
 *   This kind creates a metric per entity that has the member.
 * 
 * - EcsCounterId
 *   This metric kind will count the number of entities with a specific 
 *   (component) id. This kind creates a single metric instance for regular ids,
 *   and a metric instance per target for wildcard ids when targets is set.
 * 
 * @param world The world.
 * @param desc Metric description.
 * @return The metric entity.
 */
FLECS_API
ecs_entity_t ecs_metric_init(
    ecs_world_t *world,
    const ecs_metric_desc_t *desc);

/** Shorthand for creating a metric with ecs_metric_init.
 *
 * Example:
 *   ecs_metric(world, {
 *     .member = ecs_lookup_fullpath(world, "Position.x")
 *     .kind = EcsGauge
 *   });
 */
#define ecs_metric(world, ...)\
    ecs_metric_init(world, &(ecs_metric_desc_t) __VA_ARGS__ )

/* Module import */
FLECS_API
void FlecsMetricsImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_ALERTS
#ifdef FLECS_NO_ALERTS
#error "FLECS_NO_ALERTS failed: ALERTS is required by other addons"
#endif
/**
 * @file addons/alerts.h
 * @brief Alerts module.
 *
 * The alerts module enables applications to register alerts for when certain
 * conditions are met. Alerts are registered as queries, and automatically
 * become active when entities match the alert query.
 */

#ifdef FLECS_ALERTS

/**
 * @defgroup c_addons_alerts Alerts
 * @brief Create alerts from monitoring queries.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_ALERTS_H
#define FLECS_ALERTS_H

#ifndef FLECS_RULES
#define FLECS_RULES
#endif

#ifndef FLECS_PIPELINE
#define FLECS_PIPELINE
#endif


#define ECS_ALERT_MAX_SEVERITY_FILTERS (4)

/* Module id */
FLECS_API extern ECS_COMPONENT_DECLARE(FlecsAlerts);

/* Module components */

/** Tag added to alert, and used as first element of alert severity pair */
FLECS_API extern ECS_COMPONENT_DECLARE(EcsAlert);
FLECS_API extern ECS_COMPONENT_DECLARE(EcsAlertInstance);
FLECS_API extern ECS_COMPONENT_DECLARE(EcsAlertsActive);
FLECS_API extern ECS_COMPONENT_DECLARE(EcsAlertTimeout);

/* Alert severity tags */
FLECS_API extern ECS_TAG_DECLARE(EcsAlertInfo);
FLECS_API extern ECS_TAG_DECLARE(EcsAlertWarning);
FLECS_API extern ECS_TAG_DECLARE(EcsAlertError);
FLECS_API extern ECS_TAG_DECLARE(EcsAlertCritical);

/** Alert information. Added to each alert instance */
typedef struct EcsAlertInstance {
    char *message;
} EcsAlertInstance;

/** Map with active alerts for entity. */
typedef struct EcsAlertsActive {
    int32_t info_count;
    int32_t warning_count;
    int32_t error_count;
    ecs_map_t alerts;
} EcsAlertsActive;

typedef struct ecs_alert_severity_filter_t {
    ecs_entity_t severity; /* Severity kind */
    ecs_id_t with;         /* Component to match */
    const char *var;       /* Variable to match component on. Do not include the
                            * '$' character. Leave to NULL for $this. */
    int32_t _var_index;    /* Index of variable in filter (do not set) */
} ecs_alert_severity_filter_t;

typedef struct ecs_alert_desc_t { 
    int32_t _canary;

    /** Entity associated with alert */
    ecs_entity_t entity;

    /** Alert query. An alert will be created for each entity that matches the
     * specified query. The query must have at least one term that uses the
     * $this variable (default). */
    ecs_filter_desc_t filter;

    /** Template for alert message. This string is used to generate the alert
     * message and may refer to variables in the query result. The format for
     * the template expressions is as specified by ecs_interpolate_string.
     * 
     * Examples:
     *   "$this has Position but not Velocity"
     *   "$this has a parent entity $parent without Position"
     */
    const char *message;

    /** User friendly name. Will only be set if FLECS_DOC addon is enabled. */
    const char *doc_name;

    /** Description of alert. Will only be set if FLECS_DOC addon is enabled */
    const char *brief;

    /** Metric kind. Must be EcsAlertInfo, EcsAlertWarning, EcsAlertError or 
     * EcsAlertCritical. Defaults to EcsAlertError. */
    ecs_entity_t severity;

    /** Severity filters can be used to assign different severities to the same
     * alert. This prevents having to create multiple alerts, and allows 
     * entities to transition between severities without resetting the 
     * alert duration (optional). */
    ecs_alert_severity_filter_t severity_filters[ECS_ALERT_MAX_SEVERITY_FILTERS];

    /** The retain period specifies how long an alert must be inactive before it
     * is cleared. This makes it easier to track noisy alerts. While an alert is
     * inactive its duration won't increase. 
     * When the retain period is 0, the alert will clear immediately after it no
     * longer matches the alert query. */
    ecs_ftime_t retain_period;

    /** Alert when member value is out of range. Uses the warning/error ranges
     * assigned to the member in the MemberRanges component (optional). */
    ecs_entity_t member;

    /** (Component) id of member to monitor. If left to 0 this will be set to
     * the parent entity of the member (optional). */
    ecs_id_t id;

    /** Variable from which to fetch the member (optional). When left to NULL
     * 'id' will be obtained from $this. */
    const char *var;
} ecs_alert_desc_t;

/** Create a new alert.
 * An alert is a query that is evaluated periodically and creates alert 
 * instances for each entity that matches the query. Alerts can be used to 
 * automate detection of errors in an application.
 * 
 * Alerts are automatically cleared when a query is no longer true for an alert
 * instance. At most one alert instance will be created per matched entity.
 * 
 * Alert instances have three components:
 * - AlertInstance: contains the alert message for the instance
 * - MetricSource: contains the entity that triggered the alert
 * - MetricValue: contains how long the alert has been active
 * 
 * Alerts reuse components from the metrics addon so that alert instances can be
 * tracked and discovered as metrics. Just like metrics, alert instances are
 * created as children of the alert.
 * 
 * When an entity has active alerts, it will have the EcsAlertsActive component
 * which contains a map with active alerts for the entity. This component
 * will be automatically removed once all alerts are cleared for the entity.
 * 
 * @param world The world.
 * @param desc Alert description.
 * @return The alert entity.
 */
FLECS_API
ecs_entity_t ecs_alert_init(
    ecs_world_t *world,
    const ecs_alert_desc_t *desc);

#define ecs_alert(world, ...)\
    ecs_alert_init(world, &(ecs_alert_desc_t)__VA_ARGS__)

/** Return number of active alerts for entity.
 * When a valid alert entity is specified for the alert parameter, the operation
 * will return whether the specified alert is active for the entity. When no
 * alert is specified, the operation will return the total number of active
 * alerts for the entity.
 * 
 * @param world The world.
 * @param entity The entity.
 * @param alert The alert to test for (optional).
 * @return The number of active alerts for the entity.
 */
FLECS_API
int32_t ecs_get_alert_count(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_entity_t alert);

/** Return alert instance for specified alert.
 * This operation returns the alert instance for the specified alert. If the
 * alert is not active for the entity, the operation will return 0.
 * 
 * @param world The world.
 * @param entity The entity.
 * @param alert The alert to test for.
 * @return The alert instance for the specified alert.
 */
FLECS_API
ecs_entity_t ecs_get_alert(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_entity_t alert);

/* Module import */
FLECS_API
void FlecsAlertsImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_MONITOR
#ifdef FLECS_NO_MONITOR
#error "FLECS_NO_MONITOR failed: MONITOR is required by other addons"
#endif
/**
 * @file addons/monitor.h
 * @brief Doc module.
 *
 * The monitor module automatically tracks statistics from the stats addon and
 * stores them in components.
 */

#ifdef FLECS_MONITOR

/**
 * @defgroup c_addons_monitor Monitor
 * @brief The monitor addon periodically tracks statistics for the world and systems.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_MONITOR_H
#define FLECS_MONITOR_H

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif

#ifndef FLECS_STATS
#define FLECS_STATS
#endif


FLECS_API extern ECS_COMPONENT_DECLARE(FlecsMonitor);
FLECS_API extern ECS_COMPONENT_DECLARE(EcsWorldStats);
FLECS_API extern ECS_COMPONENT_DECLARE(EcsWorldSummary);
FLECS_API extern ECS_COMPONENT_DECLARE(EcsPipelineStats);

FLECS_API extern ecs_entity_t EcsPeriod1s;
FLECS_API extern ecs_entity_t EcsPeriod1m;
FLECS_API extern ecs_entity_t EcsPeriod1h;
FLECS_API extern ecs_entity_t EcsPeriod1d;
FLECS_API extern ecs_entity_t EcsPeriod1w;

typedef struct {
    ecs_ftime_t elapsed;
    int32_t reduce_count;
} EcsStatsHeader;

typedef struct {
    EcsStatsHeader hdr; 
    ecs_world_stats_t stats;
} EcsWorldStats;

typedef struct {
    EcsStatsHeader hdr;
    ecs_pipeline_stats_t stats;
} EcsPipelineStats;

typedef struct {
    /* Target FPS */
    double target_fps;          /**< Target FPS */

    /* Total time */
    double frame_time_total;    /**< Total time spent processing a frame */
    double system_time_total;   /**< Total time spent in systems */
    double merge_time_total;    /**< Total time spent in merges */

    /* Last frame time */
    double frame_time_last;     /**< Time spent processing a frame */
    double system_time_last;    /**< Time spent in systems */
    double merge_time_last;     /**< Time spent in merges */
} EcsWorldSummary;

/* Module import */
FLECS_API
void FlecsMonitorImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_COREDOC
#ifdef FLECS_NO_COREDOC
#error "FLECS_NO_COREDOC failed: COREDOC is required by other addons"
#endif
/**
 * @file addons/coredoc.h
 * @brief Core doc module.
 *
 * The core doc module imports documentation and reflection data for core
 * components, tags and systems.
 */

#ifdef FLECS_COREDOC

#ifndef FLECS_DOC
#define FLECS_DOC
#endif

#ifndef FLECS_META
#define FLECS_META
#endif

#ifndef FLECS_COREDOC_H
#define FLECS_COREDOC_H


/**
 * @defgroup c_addons_coredoc Coredoc
 * @brief Module that adds documentation and reflection to core entities.
 * 
 * \ingroup c_addons
 * @{
 */

/* Module import */

FLECS_API
void FlecsCoreDocImport(
    ecs_world_t *world);

/* @} */


#endif

#endif

#endif

#ifdef FLECS_DOC
#ifdef FLECS_NO_DOC
#error "FLECS_NO_DOC failed: DOC is required by other addons"
#endif
/**
 * @file addons/doc.h
 * @brief Doc module.
 *
 * The doc module allows for documenting entities (and thus components, systems)
 * by adding brief and/or detailed descriptions as components. Documentation
 * added with the doc module can be retrieved at runtime, and can be used by
 * tooling such as UIs or documentation frameworks.
 */

#ifdef FLECS_DOC

#ifndef FLECS_DOC_H
#define FLECS_DOC_H

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif


/**
 * @defgroup c_addons_doc Doc
 * @brief Utilities for documenting entities, components and systems.
 * 
 * \ingroup c_addons
 * @{
 */

FLECS_API extern const ecs_entity_t ecs_id(EcsDocDescription);
FLECS_API extern const ecs_entity_t EcsDocBrief;
FLECS_API extern const ecs_entity_t EcsDocDetail;
FLECS_API extern const ecs_entity_t EcsDocLink;
FLECS_API extern const ecs_entity_t EcsDocColor;

typedef struct EcsDocDescription {
    char *value;
} EcsDocDescription;

/** Add human-readable name to entity.
 * Contrary to entity names, human readable names do not have to be unique and
 * can contain special characters used in the query language like '*'.
 * 
 * @param world The world.
 * @param entity The entity to which to add the name.
 * @param name The name to add.
 */
FLECS_API
void ecs_doc_set_name(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *name);

/** Add brief description to entity.
 * 
 * @param world The world.
 * @param entity The entity to which to add the description.
 * @param description The description to add.
 */
FLECS_API
void ecs_doc_set_brief(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *description);

/** Add detailed description to entity.
 * 
 * @param world The world.
 * @param entity The entity to which to add the description.
 * @param description The description to add.
 */
FLECS_API
void ecs_doc_set_detail(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *description);

/** Add link to external documentation to entity.
 * 
 * @param world The world.
 * @param entity The entity to which to add the link.
 * @param link The link to add.
 */
FLECS_API
void ecs_doc_set_link(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *link);

/** Add color to entity.
 * UIs can use color as hint to improve visualizing entities.
 * 
 * @param world The world.
 * @param entity The entity to which to add the link.
 * @param color The color to add.
 */
FLECS_API
void ecs_doc_set_color(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *color);

/** Get human readable name from entity.
 * If entity does not have an explicit human readable name, this operation will
 * return the entity name.
 * 
 * To test if an entity has a human readable name, use:
 *   ecs_has_pair(world, e, ecs_id(EcsDescription), EcsName);
 * Or in C++:
 *   e.has<flecs::Description>(flecs::Name);
 * 
 * @param world The world.
 * @param entity The entity from which to get the name.
 * @return The name.
 */
FLECS_API
const char* ecs_doc_get_name(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get brief description from entity.
 * 
 * @param world The world.
 * @param entity The entity from which to get the description.
 * @return The description.
 */
FLECS_API
const char* ecs_doc_get_brief(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get detailed description from entity.
 * 
 * @param world The world.
 * @param entity The entity from which to get the description.
 * @return The description.
 */
FLECS_API
const char* ecs_doc_get_detail(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get link to external documentation from entity.
 * 
 * @param world The world.
 * @param entity The entity from which to get the link.
 * @return The link.
 */
FLECS_API
const char* ecs_doc_get_link(
    const ecs_world_t *world,
    ecs_entity_t entity);

/** Get color from entity.
 * 
 * @param world The world.
 * @param entity The entity from which to get the link.
 * @return The color.
 */
FLECS_API
const char* ecs_doc_get_color(
    const ecs_world_t *world,
    ecs_entity_t entity);

/* Module import */
FLECS_API
void FlecsDocImport(
    ecs_world_t *world);

/* @} */


#endif

#endif

#endif

#ifdef FLECS_JSON
#ifdef FLECS_NO_JSON
#error "FLECS_NO_JSON failed: JSON is required by other addons"
#endif
/**
 * @file addons/json.h
 * @brief JSON parser addon.
 *
 * Parse expression strings into component values. Entity identifiers, 
 * enumerations and bitmasks are encoded as strings.
 * 
 * See docs/JsonFormat.md for a description of the JSON format.
 */

#ifdef FLECS_JSON

#ifndef FLECS_EXPR
#define FLECS_EXPR
#endif

#ifndef FLECS_JSON_H
#define FLECS_JSON_H

/**
 * @defgroup c_addons_json Json
 * @brief Functions for serializing to/from JSON.
 * 
 * \ingroup c_addons
 * @{
 */


/** Used with ecs_ptr_from_json, ecs_entity_from_json. */
typedef struct ecs_from_json_desc_t {
    const char *name; /**< Name of expression (used for logging) */
    const char *expr; /**< Full expression (used for logging) */

    /** Callback that allows for specifying a custom lookup function. The 
     * default behavior uses ecs_lookup_fullpath */
    ecs_entity_t (*lookup_action)(
        const ecs_world_t*, 
        const char *value, 
        void *ctx);
    void *lookup_ctx;
} ecs_from_json_desc_t;

/** Parse JSON string into value.
 * This operation parses a JSON expression into the provided pointer. The
 * memory pointed to must be large enough to contain a value of the used type.
 * 
 * @param world The world.
 * @param type The type of the expression to parse.
 * @param ptr Pointer to the memory to write to.
 * @param json The JSON expression to parse.
 * @param desc Configuration parameters for deserializer.
 * @return Pointer to the character after the last one read, or NULL if failed.
 */
FLECS_API
const char* ecs_ptr_from_json(
    const ecs_world_t *world,
    ecs_entity_t type,
    void *ptr,
    const char *json,
    const ecs_from_json_desc_t *desc);

/** Parse JSON object with multiple component values into entity. The format
 * is the same as the one outputted by ecs_entity_to_json, but at the moment
 * only supports the "ids" and "values" member. 
 * 
 * @param world The world.
 * @param entity The entity to serialize to.
 * @param json The JSON expression to parse (see entity in JSON format manual).
 * @param desc Configuration parameters for deserializer.
 * @return Pointer to the character after the last one read, or NULL if failed.
 */
FLECS_API
const char* ecs_entity_from_json(
    ecs_world_t *world,
    ecs_entity_t entity,
    const char *json,
    const ecs_from_json_desc_t *desc);

/** Parse JSON object with multiple entities into the world. The format is the
 * same as the one outputted by ecs_world_to_json. 
 * 
 * @param world The world.
 * @param json The JSON expression to parse (see iterator in JSON format manual).
 */
FLECS_API
const char* ecs_world_from_json(
    ecs_world_t *world,
    const char *json,
    const ecs_from_json_desc_t *desc);

/** Serialize array into JSON string.
 * This operation serializes a value of the provided type to a JSON string. The 
 * memory pointed to must be large enough to contain a value of the used type.
 * 
 * If count is 0, the function will serialize a single value, not wrapped in
 * array brackets. If count is >= 1, the operation will serialize values to a
 * a comma-separated list inside of array brackets.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @param count The number of elements to serialize.
 * @return String with JSON expression, or NULL if failed.
 */
FLECS_API
char* ecs_array_to_json(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data,
    int32_t count);

/** Serialize array into JSON string buffer.
 * Same as ecs_array_to_json_buf, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @param count The number of elements to serialize.
 * @param buf_out The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_array_to_json_buf(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data,
    int32_t count,
    ecs_strbuf_t *buf_out);

/** Serialize value into JSON string.
 * Same as ecs_array_to_json, with count = 0.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @return String with JSON expression, or NULL if failed.
 */
FLECS_API
char* ecs_ptr_to_json(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data);

/** Serialize value into JSON string buffer.
 * Same as ecs_ptr_to_json, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @param buf_out The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_ptr_to_json_buf(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data,
    ecs_strbuf_t *buf_out);

/** Serialize type info to JSON.
 * This serializes type information to JSON, and can be used to store/transmit
 * the structure of a (component) value.
 * 
 * If the provided type does not have reflection data, "0" will be returned.
 * 
 * @param world The world.
 * @param type The type to serialize to JSON.
 * @return A JSON string with the serialized type info, or NULL if failed.
 */
FLECS_API
char* ecs_type_info_to_json(
    const ecs_world_t *world,
    ecs_entity_t type);

/** Serialize type info into JSON string buffer.
 * Same as ecs_type_info_to_json, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param type The type to serialize.
 * @param buf_out The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_type_info_to_json_buf(
    const ecs_world_t *world,
    ecs_entity_t type,
    ecs_strbuf_t *buf_out);

/** Used with ecs_iter_to_json. */
typedef struct ecs_entity_to_json_desc_t {
    bool serialize_path;       /**< Serialize full pathname */
    bool serialize_label;      /**< Serialize doc name */
    bool serialize_brief;      /**< Serialize brief doc description */
    bool serialize_link;       /**< Serialize doc link (URL) */
    bool serialize_color;      /**< Serialize doc color */
    bool serialize_ids;        /**< Serialize (component) ids */
    bool serialize_id_labels;  /**< Serialize labels of (component) ids */
    bool serialize_base;       /**< Serialize base components */
    bool serialize_private;    /**< Serialize private components */
    bool serialize_hidden;     /**< Serialize ids hidden by override */
    bool serialize_values;     /**< Serialize component values */
    bool serialize_type_info;  /**< Serialize type info (requires serialize_values) */
    bool serialize_alerts;     /**< Serialize active alerts for entity */
    ecs_entity_t serialize_refs; /**< Serialize references (incoming edges) for relationship */
    bool serialize_matches;    /**< Serialize which queries entity matches with */
} ecs_entity_to_json_desc_t;

#define ECS_ENTITY_TO_JSON_INIT (ecs_entity_to_json_desc_t){true, false,\
    false, false, false, true, false, true, false, false, false, false, false,\
    false, false }

/** Serialize entity into JSON string.
 * This creates a JSON object with the entity's (path) name, which components
 * and tags the entity has, and the component values.
 * 
 * The operation may fail if the entity contains components with invalid values.
 * 
 * @param world The world.
 * @param entity The entity to serialize to JSON.
 * @return A JSON string with the serialized entity data, or NULL if failed.
 */
FLECS_API
char* ecs_entity_to_json(
    const ecs_world_t *world,
    ecs_entity_t entity,
    const ecs_entity_to_json_desc_t *desc);

/** Serialize entity into JSON string buffer.
 * Same as ecs_entity_to_json, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param entity The entity to serialize.
 * @param buf_out The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_entity_to_json_buf(
    const ecs_world_t *world,
    ecs_entity_t entity,
    ecs_strbuf_t *buf_out,
    const ecs_entity_to_json_desc_t *desc);

/** Used with ecs_iter_to_json. */
typedef struct ecs_iter_to_json_desc_t {
    bool serialize_term_ids;        /**< Serialize query term component ids */
    bool serialize_term_labels;     /**< Serialize query term component id labels */
    bool serialize_ids;             /**< Serialize actual (matched) component ids */
    bool serialize_id_labels;       /**< Serialize actual (matched) component id labels */
    bool serialize_sources;         /**< Serialize sources */
    bool serialize_variables;       /**< Serialize variables */
    bool serialize_is_set;          /**< Serialize is_set (for optional terms) */
    bool serialize_values;          /**< Serialize component values */
    bool serialize_private;         /**< Serialize component values */
    bool serialize_entities;        /**< Serialize entities (for This terms) */
    bool serialize_entity_labels;   /**< Serialize doc name for entities */
    bool serialize_entity_ids;      /**< Serialize numerical ids for entities */
    bool serialize_entity_names;    /**< Serialize names (not paths) for entities */
    bool serialize_variable_labels; /**< Serialize doc name for variables */
    bool serialize_variable_ids;    /**< Serialize numerical ids for variables */
    bool serialize_colors;          /**< Serialize doc color for entities */
    bool measure_eval_duration;     /**< Serialize evaluation duration */
    bool serialize_type_info;       /**< Serialize type information */
    bool serialize_table;           /**< Serialize entire table vs. matched components */
} ecs_iter_to_json_desc_t;

#define ECS_ITER_TO_JSON_INIT (ecs_iter_to_json_desc_t){\
    .serialize_term_ids =        true,  \
    .serialize_term_labels =     false, \
    .serialize_ids =             true,  \
    .serialize_id_labels =       false,  \
    .serialize_sources =         true,  \
    .serialize_variables =       true,  \
    .serialize_is_set =          true,  \
    .serialize_values =          true,  \
    .serialize_entities =        true,  \
    .serialize_entity_labels =   false, \
    .serialize_entity_ids =      false, \
    .serialize_entity_names =    false, \
    .serialize_variable_labels = false, \
    .serialize_variable_ids =    false, \
    .serialize_colors =          false, \
    .measure_eval_duration =     false, \
    .serialize_type_info =       false, \
    .serialize_table =           false  \
}

/** Serialize iterator into JSON string.
 * This operation will iterate the contents of the iterator and serialize them
 * to JSON. The function accepts iterators from any source.
 * 
 * @param world The world.
 * @param iter The iterator to serialize to JSON.
 * @return A JSON string with the serialized iterator data, or NULL if failed.
 */
FLECS_API
char* ecs_iter_to_json(
    const ecs_world_t *world,
    ecs_iter_t *iter,
    const ecs_iter_to_json_desc_t *desc);

/** Serialize iterator into JSON string buffer.
 * Same as ecs_iter_to_json, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param iter The iterator to serialize.
 * @param buf_out The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_iter_to_json_buf(
    const ecs_world_t *world,
    ecs_iter_t *iter,
    ecs_strbuf_t *buf_out,
    const ecs_iter_to_json_desc_t *desc);

/** Used with ecs_iter_to_json. */
typedef struct ecs_world_to_json_desc_t {
    bool serialize_builtin;    /**< Exclude flecs modules & contents */
    bool serialize_modules;    /**< Exclude modules & contents */
} ecs_world_to_json_desc_t;

/** Serialize world into JSON string.
 * This operation iterates the contents of the world to JSON. The operation is
 * equivalent to the following code:
 * 
 * ecs_filter_t *f = ecs_filter(world, {
 *   .terms = {{ .id = EcsAny }}
 * });
 * 
 * ecs_iter_t it = ecs_filter_init(world, &f);
 * ecs_iter_to_json_desc_t desc = { .serialize_table = true };
 * ecs_iter_to_json(world, iter, &desc);
 * 
 * @param world The world to serialize.
 * @return A JSON string with the serialized iterator data, or NULL if failed.
 */
FLECS_API
char* ecs_world_to_json(
    ecs_world_t *world,
    const ecs_world_to_json_desc_t *desc);

/** Serialize world into JSON string buffer.
 * Same as ecs_world_to_json, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world to serialize.
 * @param buf_out The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_world_to_json_buf(
    ecs_world_t *world,
    ecs_strbuf_t *buf_out,
    const ecs_world_to_json_desc_t *desc);


#endif

/** @} */

#endif

#endif

#if defined(FLECS_EXPR) || defined(FLECS_META_C)
#ifndef FLECS_META
#define FLECS_META
#endif
#endif
#ifdef FLECS_UNITS
#ifdef FLECS_NO_UNITS
#error "FLECS_NO_UNITS failed: UNITS is required by other addons"
#endif
/**
 * @file addons/units.h
 * @brief Units module.
 *
 * Builtin standard units. The units addon is not imported by default, even if
 * the addon is included in the build. To import the module, do:
 *
 * In C:
 *   ECS_IMPORT(world, FlecsUnits);
 * 
 * In C++:
 *   world.import<flecs::units>();
 *
 * As a result this module behaves just like an application-defined module, 
 * which means that the ids generated for the entities inside the module are not
 * fixed, and depend on the order in which the module is imported.
 */

#ifdef FLECS_UNITS

/**
 * @defgroup c_addons_units Units.
 * @brief Common unit annotations for reflection framework.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif

#ifndef FLECS_META
#define FLECS_META
#endif

#ifndef FLECS_UNITS_H
#define FLECS_UNITS_H


/**
 * @defgroup c_addons_units_prefixes Prefixes
 * @brief Prefixes to indicate unit count (e.g. Kilo, Mega)
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsUnitPrefixes); /* Parent scope for prefixes */

FLECS_API extern ECS_DECLARE(EcsYocto);
FLECS_API extern ECS_DECLARE(EcsZepto);
FLECS_API extern ECS_DECLARE(EcsAtto);
FLECS_API extern ECS_DECLARE(EcsFemto);
FLECS_API extern ECS_DECLARE(EcsPico);
FLECS_API extern ECS_DECLARE(EcsNano);
FLECS_API extern ECS_DECLARE(EcsMicro);
FLECS_API extern ECS_DECLARE(EcsMilli);
FLECS_API extern ECS_DECLARE(EcsCenti);
FLECS_API extern ECS_DECLARE(EcsDeci);
FLECS_API extern ECS_DECLARE(EcsDeca);
FLECS_API extern ECS_DECLARE(EcsHecto);
FLECS_API extern ECS_DECLARE(EcsKilo);
FLECS_API extern ECS_DECLARE(EcsMega);
FLECS_API extern ECS_DECLARE(EcsGiga);
FLECS_API extern ECS_DECLARE(EcsTera);
FLECS_API extern ECS_DECLARE(EcsPeta);
FLECS_API extern ECS_DECLARE(EcsExa);
FLECS_API extern ECS_DECLARE(EcsZetta);
FLECS_API extern ECS_DECLARE(EcsYotta);

FLECS_API extern ECS_DECLARE(EcsKibi);
FLECS_API extern ECS_DECLARE(EcsMebi);
FLECS_API extern ECS_DECLARE(EcsGibi);
FLECS_API extern ECS_DECLARE(EcsTebi);
FLECS_API extern ECS_DECLARE(EcsPebi);
FLECS_API extern ECS_DECLARE(EcsExbi);
FLECS_API extern ECS_DECLARE(EcsZebi);
FLECS_API extern ECS_DECLARE(EcsYobi);

/** @} */

/**
 * @defgroup c_addons_units_duration Duration
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsDuration);
FLECS_API extern     ECS_DECLARE(EcsPicoSeconds);
FLECS_API extern     ECS_DECLARE(EcsNanoSeconds);
FLECS_API extern     ECS_DECLARE(EcsMicroSeconds);
FLECS_API extern     ECS_DECLARE(EcsMilliSeconds);
FLECS_API extern     ECS_DECLARE(EcsSeconds);
FLECS_API extern     ECS_DECLARE(EcsMinutes);
FLECS_API extern     ECS_DECLARE(EcsHours);
FLECS_API extern     ECS_DECLARE(EcsDays);

/** @} */

/**
 * @defgroup c_addons_units_time Time
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsTime);
FLECS_API extern     ECS_DECLARE(EcsDate);

/** @} */

/**
 * @defgroup c_addons_units_mass Mass
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsMass);
FLECS_API extern     ECS_DECLARE(EcsGrams);
FLECS_API extern     ECS_DECLARE(EcsKiloGrams);

/** @} */

/**
 * @defgroup c_addons_units_electric_Current Electric Current
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsElectricCurrent);
FLECS_API extern     ECS_DECLARE(EcsAmpere);

/** @} */

/**
 * @defgroup c_addons_units_amount Amount
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsAmount);
FLECS_API extern     ECS_DECLARE(EcsMole);

/** @} */

/**
 * @defgroup c_addons_units_luminous_intensity Luminous Intensity
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsLuminousIntensity);
FLECS_API extern     ECS_DECLARE(EcsCandela);

/** @} */

/**
 * @defgroup c_addons_units_force Force
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsForce);
FLECS_API extern     ECS_DECLARE(EcsNewton);

/** @} */

/**
 * @defgroup c_addons_units_length Length
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsLength);
FLECS_API extern     ECS_DECLARE(EcsMeters);
FLECS_API extern         ECS_DECLARE(EcsPicoMeters);
FLECS_API extern         ECS_DECLARE(EcsNanoMeters);
FLECS_API extern         ECS_DECLARE(EcsMicroMeters);
FLECS_API extern         ECS_DECLARE(EcsMilliMeters);
FLECS_API extern         ECS_DECLARE(EcsCentiMeters);
FLECS_API extern         ECS_DECLARE(EcsKiloMeters);
FLECS_API extern     ECS_DECLARE(EcsMiles);
FLECS_API extern     ECS_DECLARE(EcsPixels);

/** @} */

/**
 * @defgroup c_addons_units_pressure Pressure
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsPressure);
FLECS_API extern     ECS_DECLARE(EcsPascal);
FLECS_API extern     ECS_DECLARE(EcsBar);

/** @} */

/**
 * @defgroup c_addons_units_speed Speed
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsSpeed);
FLECS_API extern     ECS_DECLARE(EcsMetersPerSecond);
FLECS_API extern     ECS_DECLARE(EcsKiloMetersPerSecond);
FLECS_API extern     ECS_DECLARE(EcsKiloMetersPerHour);
FLECS_API extern     ECS_DECLARE(EcsMilesPerHour);

/** @} */

/**
 * @defgroup c_addons_units_temperature Temperature
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsTemperature);
FLECS_API extern     ECS_DECLARE(EcsKelvin);
FLECS_API extern     ECS_DECLARE(EcsCelsius);
FLECS_API extern     ECS_DECLARE(EcsFahrenheit);

/** @} */

/**
 * @defgroup c_addons_units_data Data
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsData);
FLECS_API extern     ECS_DECLARE(EcsBits);
FLECS_API extern         ECS_DECLARE(EcsKiloBits);
FLECS_API extern         ECS_DECLARE(EcsMegaBits);
FLECS_API extern         ECS_DECLARE(EcsGigaBits);
FLECS_API extern     ECS_DECLARE(EcsBytes);
FLECS_API extern         ECS_DECLARE(EcsKiloBytes);
FLECS_API extern         ECS_DECLARE(EcsMegaBytes);
FLECS_API extern         ECS_DECLARE(EcsGigaBytes);
FLECS_API extern         ECS_DECLARE(EcsKibiBytes);
FLECS_API extern         ECS_DECLARE(EcsMebiBytes);
FLECS_API extern         ECS_DECLARE(EcsGibiBytes);

/** @} */

/**
 * @defgroup c_addons_units_datarate Data Rate
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsDataRate);
FLECS_API extern     ECS_DECLARE(EcsBitsPerSecond);
FLECS_API extern     ECS_DECLARE(EcsKiloBitsPerSecond);
FLECS_API extern     ECS_DECLARE(EcsMegaBitsPerSecond);
FLECS_API extern     ECS_DECLARE(EcsGigaBitsPerSecond);
FLECS_API extern     ECS_DECLARE(EcsBytesPerSecond);
FLECS_API extern     ECS_DECLARE(EcsKiloBytesPerSecond);
FLECS_API extern     ECS_DECLARE(EcsMegaBytesPerSecond);
FLECS_API extern     ECS_DECLARE(EcsGigaBytesPerSecond);

/** @} */

/**
 * @defgroup c_addons_units_duration Duration
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsAngle);
FLECS_API extern     ECS_DECLARE(EcsRadians);
FLECS_API extern     ECS_DECLARE(EcsDegrees);

/** @} */

/**
 * @defgroup c_addons_units_angle Angle
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsFrequency);
FLECS_API extern     ECS_DECLARE(EcsHertz);
FLECS_API extern     ECS_DECLARE(EcsKiloHertz);
FLECS_API extern     ECS_DECLARE(EcsMegaHertz);
FLECS_API extern     ECS_DECLARE(EcsGigaHertz);

/** @} */

/**
 * @defgroup c_addons_units_uri Uri
 * 
 * \ingroup c_addons_units
 * @{
 */

FLECS_API extern ECS_DECLARE(EcsUri);
FLECS_API extern     ECS_DECLARE(EcsUriHyperlink);
FLECS_API extern     ECS_DECLARE(EcsUriImage);
FLECS_API extern     ECS_DECLARE(EcsUriFile);

/** @} */

FLECS_API extern ECS_DECLARE(EcsAcceleration);
FLECS_API extern ECS_DECLARE(EcsPercentage);
FLECS_API extern ECS_DECLARE(EcsBel);
FLECS_API extern ECS_DECLARE(EcsDeciBel);

////////////////////////////////////////////////////////////////////////////////
//// Module
////////////////////////////////////////////////////////////////////////////////

FLECS_API
void FlecsUnitsImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_META
#ifdef FLECS_NO_META
#error "FLECS_NO_META failed: META is required by other addons"
#endif
/**
 * @file addons/meta.h
 * @brief Meta addon.
 *
 * The meta addon enables reflecting on component data. Types are stored as
 * entities, with components that store the reflection data. A type has at least
 * two components:
 * 
 * - EcsComponent: core component, contains size & alignment
 * - EcsMetaType:  component that indicates what kind of type the entity is
 *
 * Additionally the type may have an additional component that contains the
 * reflection data for the type. For example, structs have these components:
 * 
 * - EcsComponent
 * - EcsMetaType
 * - EcsStruct
 * 
 * Structs can be populated by adding child entities with the EcsMember 
 * component. Adding a child with a Member component to an entity will 
 * automatically add the EcsStruct component to the parent.
 * 
 * Enums/bitmasks can be populated by adding child entities with the Constant
 * tag. By default constants are automatically assigned values when they are
 * added to the enum/bitmask. The parent entity must have the EcsEnum or
 * EcsBitmask component before adding the constants.
 * 
 * To create enum constants with a manual value, set (Constant, i32) to the
 * desired value. To create bitmask constants with a manual value, set
 * (Constant, u32) to the desired value. Constants with manual values should not
 * conflict with other constants.
 * 
 * The _init APIs are convenience wrappers around creating the entities and
 * components for the types.
 * 
 * When a type is created it automatically receives the EcsComponent and 
 * EcsMetaType components. The former means that the resulting type can be
 * used as a regular component:
 * 
 * // Create Position type
 * ecs_entity_t pos = ecs_struct_init(world, &(ecs_struct_desc_t){
 *  .entity.name = "Position",
 *  .members = {
 *       {"x", ecs_id(ecs_f32_t)},
 *       {"y", ecs_id(ecs_f32_t)}
 *   }
 * });
 *
 * // Create entity with Position component
 * ecs_entity_t e = ecs_new_w_id(world, pos);
 * 
 * Type entities do not have to be named.
 */

#ifdef FLECS_META

/**
 * @defgroup c_addons_meta Meta
 * @brief Flecs reflection framework.
 * 
 * \ingroup c_addons
 * @{
 */

#include <stddef.h>

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif

#ifndef FLECS_META_H
#define FLECS_META_H


#define ECS_MEMBER_DESC_CACHE_SIZE (32)

/** Primitive type definitions.
 * These typedefs allow the builtin primitives to be used as regular components:
 *   ecs_set(world, e, ecs_i32_t, {10});
 * 
 * Or a more useful example (create an enum constant with a manual value):
 *   ecs_set_pair_object(world, e, EcsConstant, ecs_i32_t, {10});
 */
typedef bool ecs_bool_t;
typedef char ecs_char_t;
typedef unsigned char ecs_byte_t;
typedef uint8_t ecs_u8_t;
typedef uint16_t ecs_u16_t;
typedef uint32_t ecs_u32_t;
typedef uint64_t ecs_u64_t;
typedef uintptr_t ecs_uptr_t;
typedef int8_t ecs_i8_t;
typedef int16_t ecs_i16_t;
typedef int32_t ecs_i32_t;
typedef int64_t ecs_i64_t;
typedef intptr_t ecs_iptr_t;
typedef float ecs_f32_t;
typedef double ecs_f64_t;
typedef char* ecs_string_t;

/* Meta module component ids */
FLECS_API extern const ecs_entity_t ecs_id(EcsMetaType);
FLECS_API extern const ecs_entity_t ecs_id(EcsMetaTypeSerialized);
FLECS_API extern const ecs_entity_t ecs_id(EcsPrimitive);
FLECS_API extern const ecs_entity_t ecs_id(EcsEnum);
FLECS_API extern const ecs_entity_t ecs_id(EcsBitmask);
FLECS_API extern const ecs_entity_t ecs_id(EcsMember);
FLECS_API extern const ecs_entity_t ecs_id(EcsMemberRanges);
FLECS_API extern const ecs_entity_t ecs_id(EcsStruct);
FLECS_API extern const ecs_entity_t ecs_id(EcsArray);
FLECS_API extern const ecs_entity_t ecs_id(EcsVector);
FLECS_API extern const ecs_entity_t ecs_id(EcsOpaque);
FLECS_API extern const ecs_entity_t ecs_id(EcsUnit);
FLECS_API extern const ecs_entity_t ecs_id(EcsUnitPrefix);
FLECS_API extern const ecs_entity_t EcsConstant;
FLECS_API extern const ecs_entity_t EcsQuantity;

/* Primitive type component ids */
FLECS_API extern const ecs_entity_t ecs_id(ecs_bool_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_char_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_byte_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_u8_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_u16_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_u32_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_u64_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_uptr_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_i8_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_i16_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_i32_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_i64_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_iptr_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_f32_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_f64_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_string_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_entity_t);
FLECS_API extern const ecs_entity_t ecs_id(ecs_id_t);

/** Type kinds supported by meta addon */
typedef enum ecs_type_kind_t {
    EcsPrimitiveType,
    EcsBitmaskType,
    EcsEnumType,
    EcsStructType,
    EcsArrayType,
    EcsVectorType,
    EcsOpaqueType,
    EcsTypeKindLast = EcsOpaqueType
} ecs_type_kind_t;

/** Component that is automatically added to every type with the right kind. */
typedef struct EcsMetaType {
    ecs_type_kind_t kind;
    bool existing;         /**< Did the type exist or is it populated from reflection */
    bool partial;          /**< Is the reflection data a partial type description */
} EcsMetaType;

/** Primitive type kinds supported by meta addon */
typedef enum ecs_primitive_kind_t {
    EcsBool = 1,
    EcsChar,
    EcsByte,
    EcsU8,
    EcsU16,
    EcsU32,
    EcsU64,
    EcsI8,
    EcsI16,
    EcsI32,
    EcsI64,
    EcsF32,
    EcsF64,
    EcsUPtr,
    EcsIPtr,
    EcsString,
    EcsEntity,
    EcsId,
    EcsPrimitiveKindLast = EcsId
} ecs_primitive_kind_t;

/** Component added to primitive types */
typedef struct EcsPrimitive {
    ecs_primitive_kind_t kind;
} EcsPrimitive;

/** Component added to member entities */
typedef struct EcsMember {
    ecs_entity_t type;
    int32_t count;
    ecs_entity_t unit;
    int32_t offset;
} EcsMember;

/** Type expressing a range for a member value */
typedef struct ecs_member_value_range_t {
    double min;
    double max;
} ecs_member_value_range_t;

/** Component added to member entities to express valid value ranges */
typedef struct EcsMemberRanges {
    ecs_member_value_range_t value;
    ecs_member_value_range_t warning;
    ecs_member_value_range_t error;
} EcsMemberRanges;

/** Element type of members vector in EcsStruct */
typedef struct ecs_member_t {
    /** Must be set when used with ecs_struct_desc_t */
    const char *name;
    ecs_entity_t type;

    /** May be set when used with ecs_struct_desc_t */
    int32_t count;
    int32_t offset;

    /** May be set when used with ecs_struct_desc_t, will be auto-populated if
     * type entity is also a unit */
    ecs_entity_t unit;

    /** Numerical range that specifies which values member can assume. This 
     * range may be used by UI elements such as a progress bar or slider. The
     * value of a member should not exceed this range. */
    ecs_member_value_range_t range;

    /** Numerical range outside of which the value represents an error. This 
     * range may be used by UI elements to style a value. */
    ecs_member_value_range_t error_range;

    /** Numerical range outside of which the value represents an warning. This 
     * range may be used by UI elements to style a value. */
    ecs_member_value_range_t warning_range;

    /** Should not be set by ecs_struct_desc_t */
    ecs_size_t size;
    ecs_entity_t member;
} ecs_member_t;

/** Component added to struct type entities */
typedef struct EcsStruct {
    /** Populated from child entities with Member component */
    ecs_vec_t members; /* vector<ecs_member_t> */
} EcsStruct;

typedef struct ecs_enum_constant_t {
    /** Must be set when used with ecs_enum_desc_t */
    const char *name;

    /** May be set when used with ecs_enum_desc_t */
    int32_t value;

    /** Should not be set by ecs_enum_desc_t */
    ecs_entity_t constant;
} ecs_enum_constant_t;

/** Component added to enum type entities */
typedef struct EcsEnum {
    /** Populated from child entities with Constant component */
    ecs_map_t constants; /* map<i32_t, ecs_enum_constant_t> */
} EcsEnum;

typedef struct ecs_bitmask_constant_t {
    /** Must be set when used with ecs_bitmask_desc_t */
    const char *name;

    /** May be set when used with ecs_bitmask_desc_t */
    ecs_flags32_t value;

    /** Should not be set by ecs_bitmask_desc_t */
    ecs_entity_t constant;
} ecs_bitmask_constant_t;

/** Component added to bitmask type entities */
typedef struct EcsBitmask {
    /* Populated from child entities with Constant component */
    ecs_map_t constants; /* map<u32_t, ecs_bitmask_constant_t> */
} EcsBitmask;

/** Component added to array type entities */
typedef struct EcsArray {
    ecs_entity_t type; /**< Element type */
    int32_t count;     /**< Number of elements */
} EcsArray;

/** Component added to vector type entities */
typedef struct EcsVector {
    ecs_entity_t type; /**< Element type */
} EcsVector;


/* Opaque type support */

#if !defined(__cplusplus) || !defined(FLECS_CPP)

/** Serializer interface */
typedef struct ecs_serializer_t {
    /* Serialize value */
    int (*value)(
        const struct ecs_serializer_t *ser, /**< Serializer */
        ecs_entity_t type,                  /**< Type of the value to serialize */
        const void *value);                 /**< Pointer to the value to serialize */

    /* Serialize member */
    int (*member)(
        const struct ecs_serializer_t *ser, /**< Serializer */
        const char *member);                /**< Member name */

    const ecs_world_t *world;
    void *ctx;
} ecs_serializer_t;

#endif

/** Callback invoked serializing an opaque type. */
typedef int (*ecs_meta_serialize_t)(
    const ecs_serializer_t *ser,
    const void *src);                  /**< Pointer to value to serialize */

typedef struct EcsOpaque {
    ecs_entity_t as_type;              /**< Type that describes the serialized output */
    ecs_meta_serialize_t serialize;    /**< Serialize action */

    /* Deserializer interface 
     * Only override the callbacks that are valid for the opaque type. If a
     * deserializer attempts to assign a value type that is not supported by the
     * interface, a conversion error is thrown.
     */

    /** Assign bool value */
    void (*assign_bool)(
        void *dst, 
        bool value);

    /** Assign char value */
    void (*assign_char)(
        void *dst, 
        char value);

    /** Assign int value */
    void (*assign_int)(
        void *dst, 
        int64_t value);

    /** Assign unsigned int value */
    void (*assign_uint)(
        void *dst, 
        uint64_t value);

    /** Assign float value */
    void (*assign_float)(
        void *dst, 
        double value);

    /** Assign string value */
    void (*assign_string)(
        void *dst, 
        const char *value);

    /** Assign entity value */
    void (*assign_entity)(
        void *dst,
        ecs_world_t *world,
        ecs_entity_t entity);

    /** Assign (component) id value */
    void (*assign_id)(
        void *dst,
        ecs_world_t *world,
        ecs_id_t id);

    /** Assign null value */
    void (*assign_null)(
        void *dst);

    /** Clear collection elements */
    void (*clear)(
        void *dst);

    /** Ensure & get collection element */
    void* (*ensure_element)(
        void *dst, 
        size_t elem);

    /** Ensure & get element */
    void* (*ensure_member)(
        void *dst, 
        const char *member);

    /** Return number of elements */
    size_t (*count)(
        const void *dst);
    
    /** Resize to number of elements */
    void (*resize)(
        void *dst, 
        size_t count);
} EcsOpaque;


/* Units */

/* Helper type to describe translation between two units. Note that this
 * is not intended as a generic approach to unit conversions (e.g. from celsius
 * to fahrenheit) but to translate between units that derive from the same base 
 * (e.g. meters to kilometers). 
 * 
 * Note that power is applied to the factor. When describing a translation of
 * 1000, either use {factor = 1000, power = 1} or {factor = 1, power = 3}. */
typedef struct ecs_unit_translation_t {
    int32_t factor; /**< Factor to apply (e.g. "1000", "1000000", "1024") */
    int32_t power;  /**< Power to apply to factor (e.g. "1", "3", "-9") */
} ecs_unit_translation_t;

typedef struct EcsUnit {
    char *symbol;
    ecs_entity_t prefix; /**< Order of magnitude prefix relative to derived */
    ecs_entity_t base;   /**< Base unit (e.g. "meters") */
    ecs_entity_t over;   /**< Over unit (e.g. "per second") */
    ecs_unit_translation_t translation; /**< Translation for derived unit */
} EcsUnit;

typedef struct EcsUnitPrefix {
    char *symbol;        /**< Symbol of prefix (e.g. "K", "M", "Ki") */
    ecs_unit_translation_t translation; /**< Translation of prefix */
} EcsUnitPrefix;


/* Serializer utilities */

typedef enum ecs_meta_type_op_kind_t {
    EcsOpArray,
    EcsOpVector,
    EcsOpOpaque,
    EcsOpPush,
    EcsOpPop,

    EcsOpScope, /**< Marks last constant that can open/close a scope */

    EcsOpEnum,
    EcsOpBitmask,

    EcsOpPrimitive, /**< Marks first constant that's a primitive */

    EcsOpBool,
    EcsOpChar,
    EcsOpByte,
    EcsOpU8,
    EcsOpU16,
    EcsOpU32,
    EcsOpU64,
    EcsOpI8,
    EcsOpI16,
    EcsOpI32,
    EcsOpI64,
    EcsOpF32,
    EcsOpF64,
    EcsOpUPtr,
    EcsOpIPtr,
    EcsOpString,
    EcsOpEntity,
    EcsOpId,
    EcsMetaTypeOpKindLast = EcsOpId
} ecs_meta_type_op_kind_t;

typedef struct ecs_meta_type_op_t {
    ecs_meta_type_op_kind_t kind;
    ecs_size_t offset;      /**< Offset of current field */
    int32_t count;
    const char *name;       /**< Name of value (only used for struct members) */
    int32_t op_count;       /**< Number of operations until next field or end */
    ecs_size_t size;        /**< Size of type of operation */
    ecs_entity_t type;      /**< Type entity */
    int32_t member_index;   /**< Index of member in struct */
    ecs_hashmap_t *members; /**< string -> member index (structs only) */
} ecs_meta_type_op_t;

typedef struct EcsMetaTypeSerialized {
    ecs_vec_t ops;      /**< vector<ecs_meta_type_op_t> */
} EcsMetaTypeSerialized;


/* Deserializer utilities */

#define ECS_META_MAX_SCOPE_DEPTH (32) /* >32 levels of nesting is not sane */

typedef struct ecs_meta_scope_t {
    ecs_entity_t type;        /**< The type being iterated */
    ecs_meta_type_op_t *ops;  /**< The type operations (see ecs_meta_type_op_t) */
    int32_t op_count;         /**< Number of operations in ops array to process */
    int32_t op_cur;           /**< Current operation */
    int32_t elem_cur;         /**< Current element (for collections) */
    int32_t prev_depth;       /**< Depth to restore, in case dotmember was used */
    void *ptr;                /**< Pointer to the value being iterated */

    const EcsComponent *comp; /**< Pointer to component, in case size/alignment is needed */
    const EcsOpaque *opaque;  /**< Opaque type interface */ 
    ecs_vec_t *vector;        /**< Current vector, in case a vector is iterated */
    ecs_hashmap_t *members;   /**< string -> member index */
    bool is_collection;       /**< Is the scope iterating elements? */
    bool is_inline_array;     /**< Is the scope iterating an inline array? */
    bool is_empty_scope;      /**< Was scope populated (for collections) */
} ecs_meta_scope_t;

/** Type that enables iterating/populating a value using reflection data */
typedef struct ecs_meta_cursor_t {
    const ecs_world_t *world;
    ecs_meta_scope_t scope[ECS_META_MAX_SCOPE_DEPTH];
    int32_t depth;
    bool valid;
    bool is_primitive_scope;  /**< If in root scope, this allows for a push for primitive types */

    /* Custom entity lookup action for overriding default ecs_lookup_fullpath */
    ecs_entity_t (*lookup_action)(const ecs_world_t*, const char*, void*);
    void *lookup_ctx;
} ecs_meta_cursor_t;

FLECS_API
ecs_meta_cursor_t ecs_meta_cursor(
    const ecs_world_t *world,
    ecs_entity_t type,
    void *ptr);

/** Get pointer to current field */
FLECS_API
void* ecs_meta_get_ptr(
    ecs_meta_cursor_t *cursor);

/** Move cursor to next field */
FLECS_API
int ecs_meta_next(
    ecs_meta_cursor_t *cursor);

/** Move cursor to a element */
FLECS_API
int ecs_meta_elem(
    ecs_meta_cursor_t *cursor,
    int32_t elem);

/** Move cursor to member */
FLECS_API
int ecs_meta_member(
    ecs_meta_cursor_t *cursor,
    const char *name);

/** Move cursor to member, supports dot-separated nested members */
FLECS_API
int ecs_meta_dotmember(
    ecs_meta_cursor_t *cursor,
    const char *name);

/** Push a scope (required/only valid for structs & collections) */
FLECS_API
int ecs_meta_push(
    ecs_meta_cursor_t *cursor);

/** Pop a struct or collection scope (must follow a push) */
FLECS_API
int ecs_meta_pop(
    ecs_meta_cursor_t *cursor);

/** Is the current scope a collection? */
FLECS_API
bool ecs_meta_is_collection(
    const ecs_meta_cursor_t *cursor);

/** Get type of current element. */
FLECS_API
ecs_entity_t ecs_meta_get_type(
    const ecs_meta_cursor_t *cursor);

/** Get unit of current element. */
FLECS_API
ecs_entity_t ecs_meta_get_unit(
    const ecs_meta_cursor_t *cursor);

/** Get member name of current member */
FLECS_API
const char* ecs_meta_get_member(
    const ecs_meta_cursor_t *cursor);

/** Get member entity of current member */
FLECS_API
ecs_entity_t ecs_meta_get_member_id(
    const ecs_meta_cursor_t *cursor);

/* The set functions assign the field with the specified value. If the value
 * does not have the same type as the field, it will be cased to the field type.
 * If no valid conversion is available, the operation will fail. */

/** Set field with boolean value */
FLECS_API
int ecs_meta_set_bool(
    ecs_meta_cursor_t *cursor,
    bool value);

/** Set field with char value */
FLECS_API
int ecs_meta_set_char(
    ecs_meta_cursor_t *cursor,
    char value);

/** Set field with int value */
FLECS_API
int ecs_meta_set_int(
    ecs_meta_cursor_t *cursor,
    int64_t value);

/** Set field with uint value */
FLECS_API
int ecs_meta_set_uint(
    ecs_meta_cursor_t *cursor,
    uint64_t value);

/** Set field with float value */
FLECS_API
int ecs_meta_set_float(
    ecs_meta_cursor_t *cursor,
    double value);

/** Set field with string value */
FLECS_API
int ecs_meta_set_string(
    ecs_meta_cursor_t *cursor,
    const char *value);

/** Set field with string literal value (has enclosing "") */
FLECS_API
int ecs_meta_set_string_literal(
    ecs_meta_cursor_t *cursor,
    const char *value);

/** Set field with entity value */
FLECS_API
int ecs_meta_set_entity(
    ecs_meta_cursor_t *cursor,
    ecs_entity_t value);

/** Set field with (component) id value */
FLECS_API
int ecs_meta_set_id(
    ecs_meta_cursor_t *cursor,
    ecs_id_t value);

/** Set field with (component) id value */
FLECS_API
int ecs_meta_set_component(
    ecs_meta_cursor_t *cursor,
    ecs_id_t value);

/** Set field with null value */
FLECS_API
int ecs_meta_set_null(
    ecs_meta_cursor_t *cursor);

/** Set field with dynamic value */
FLECS_API
int ecs_meta_set_value(
    ecs_meta_cursor_t *cursor,
    const ecs_value_t *value);

/* Functions for getting members. */

/** Get field value as boolean. */
FLECS_API
bool ecs_meta_get_bool(
    const ecs_meta_cursor_t *cursor);

/** Get field value as char. */
FLECS_API
char ecs_meta_get_char(
    const ecs_meta_cursor_t *cursor);

/** Get field value as signed integer. */
FLECS_API
int64_t ecs_meta_get_int(
    const ecs_meta_cursor_t *cursor);

/** Get field value as unsigned integer. */
FLECS_API
uint64_t ecs_meta_get_uint(
    const ecs_meta_cursor_t *cursor);

/** Get field value as float. */
FLECS_API
double ecs_meta_get_float(
    const ecs_meta_cursor_t *cursor);

/** Get field value as string. 
 * This operation does not perform conversions. If the field is not a string,
 * this operation will fail.
 */
FLECS_API
const char* ecs_meta_get_string(
    const ecs_meta_cursor_t *cursor);

/** Get field value as entity. 
 * This operation does not perform conversions. */
FLECS_API
ecs_entity_t ecs_meta_get_entity(
    const ecs_meta_cursor_t *cursor);

/** Get field value as (component) id. 
 * This operation can convert from an entity. */
ecs_id_t ecs_meta_get_id(
    const ecs_meta_cursor_t *cursor);

/** Convert pointer of primitive kind to float. */
FLECS_API
double ecs_meta_ptr_to_float(
    ecs_primitive_kind_t type_kind,
    const void *ptr);

/* API functions for creating meta types */

/** Used with ecs_primitive_init. */
typedef struct ecs_primitive_desc_t {
    ecs_entity_t entity; /**< Existing entity to use for type (optional) */
    ecs_primitive_kind_t kind;
} ecs_primitive_desc_t;

/** Create a new primitive type */
FLECS_API
ecs_entity_t ecs_primitive_init(
    ecs_world_t *world,
    const ecs_primitive_desc_t *desc);

/** Used with ecs_enum_init. */
typedef struct ecs_enum_desc_t {
    ecs_entity_t entity; /**< Existing entity to use for type (optional) */
    ecs_enum_constant_t constants[ECS_MEMBER_DESC_CACHE_SIZE];
} ecs_enum_desc_t;

/** Create a new enum type */
FLECS_API
ecs_entity_t ecs_enum_init(
    ecs_world_t *world,
    const ecs_enum_desc_t *desc);


/** Used with ecs_bitmask_init. */
typedef struct ecs_bitmask_desc_t {
    ecs_entity_t entity; /**< Existing entity to use for type (optional) */
    ecs_bitmask_constant_t constants[ECS_MEMBER_DESC_CACHE_SIZE];
} ecs_bitmask_desc_t;

/** Create a new bitmask type */
FLECS_API
ecs_entity_t ecs_bitmask_init(
    ecs_world_t *world,
    const ecs_bitmask_desc_t *desc);


/** Used with ecs_array_init. */
typedef struct ecs_array_desc_t {
    ecs_entity_t entity; /**< Existing entity to use for type (optional) */
    ecs_entity_t type;
    int32_t count;
} ecs_array_desc_t;

/** Create a new array type */
FLECS_API
ecs_entity_t ecs_array_init(
    ecs_world_t *world,
    const ecs_array_desc_t *desc);


/** Used with ecs_vector_init. */
typedef struct ecs_vector_desc_t {
    ecs_entity_t entity; /**< Existing entity to use for type (optional) */
    ecs_entity_t type;
} ecs_vector_desc_t;

/** Create a new vector type */
FLECS_API
ecs_entity_t ecs_vector_init(
    ecs_world_t *world,
    const ecs_vector_desc_t *desc);


/** Used with ecs_struct_init. */
typedef struct ecs_struct_desc_t {
    ecs_entity_t entity; /**< Existing entity to use for type (optional) */
    ecs_member_t members[ECS_MEMBER_DESC_CACHE_SIZE];
} ecs_struct_desc_t;

/** Create a new struct type */
FLECS_API
ecs_entity_t ecs_struct_init(
    ecs_world_t *world,
    const ecs_struct_desc_t *desc);

/** Used with ecs_opaque_init. */
typedef struct ecs_opaque_desc_t {
    ecs_entity_t entity;
    EcsOpaque type;
} ecs_opaque_desc_t;

/** Create a new opaque type.
 * Opaque types are types of which the layout doesn't match what can be modelled
 * with the primitives of the meta framework, but which have a structure
 * that can be described with meta primitives. Typical examples are STL types
 * such as std::string or std::vector, types with a nontrivial layout, and types
 * that only expose getter/setter methods.
 * 
 * An opaque type is a combination of a serialization function, and a handle to
 * a meta type which describes the structure of the serialized output. For
 * example, an opaque type for std::string would have a serializer function that
 * accesses .c_str(), and with type ecs_string_t.
 * 
 * The serializer callback accepts a serializer object and a pointer to the 
 * value of the opaque type to be serialized. The serializer has two methods:
 * 
 * - value, which serializes a value (such as .c_str())
 * - member, which specifies a member to be serialized (in the case of a struct)
 */
FLECS_API
ecs_entity_t ecs_opaque_init(
    ecs_world_t *world,
    const ecs_opaque_desc_t *desc);

/** Used with ecs_unit_init. */
typedef struct ecs_unit_desc_t {
    /** Existing entity to associate with unit (optional) */
    ecs_entity_t entity;

    /** Unit symbol, e.g. "m", "%", "g". (optional) */
    const char *symbol;

    /** Unit quantity, e.g. distance, percentage, weight. (optional) */
    ecs_entity_t quantity;

    /** Base unit, e.g. "meters" (optional) */
    ecs_entity_t base;

    /** Over unit, e.g. "per second" (optional) */
    ecs_entity_t over;

    /** Translation to apply to derived unit (optional) */
    ecs_unit_translation_t translation;

    /** Prefix indicating order of magnitude relative to the derived unit. If set
     * together with "translation", the values must match. If translation is not
     * set, setting prefix will autopopulate it.
     * Additionally, setting the prefix will enforce that the symbol (if set)
     * is consistent with the prefix symbol + symbol of the derived unit. If the
     * symbol is not set, it will be auto populated. */
    ecs_entity_t prefix;
} ecs_unit_desc_t;

/** Create a new unit */
FLECS_API
ecs_entity_t ecs_unit_init(
    ecs_world_t *world,
    const ecs_unit_desc_t *desc);

/** Used with ecs_unit_prefix_init. */
typedef struct ecs_unit_prefix_desc_t {
    /** Existing entity to associate with unit prefix (optional) */
    ecs_entity_t entity;

    /** Unit symbol, e.g. "m", "%", "g". (optional) */
    const char *symbol;

    /** Translation to apply to derived unit (optional) */
    ecs_unit_translation_t translation;
} ecs_unit_prefix_desc_t;

/** Create a new unit prefix */
FLECS_API
ecs_entity_t ecs_unit_prefix_init(
    ecs_world_t *world,
    const ecs_unit_prefix_desc_t *desc);

/** Create a new quantity */
FLECS_API
ecs_entity_t ecs_quantity_init(
    ecs_world_t *world,
    const ecs_entity_desc_t *desc);

/* Convenience macros */

#define ecs_primitive(world, ...)\
    ecs_primitive_init(world, &(ecs_primitive_desc_t) __VA_ARGS__ )

#define ecs_enum(world, ...)\
    ecs_enum_init(world, &(ecs_enum_desc_t) __VA_ARGS__ )

#define ecs_bitmask(world, ...)\
    ecs_bitmask_init(world, &(ecs_bitmask_desc_t) __VA_ARGS__ )

#define ecs_array(world, ...)\
    ecs_array_init(world, &(ecs_array_desc_t) __VA_ARGS__ )

#define ecs_vector(world, ...)\
    ecs_vector_init(world, &(ecs_vector_desc_t) __VA_ARGS__ )

#define ecs_opaque(world, ...)\
    ecs_opaque_init(world, &(ecs_opaque_desc_t) __VA_ARGS__ )

#define ecs_struct(world, ...)\
    ecs_struct_init(world, &(ecs_struct_desc_t) __VA_ARGS__ )

#define ecs_unit(world, ...)\
    ecs_unit_init(world, &(ecs_unit_desc_t) __VA_ARGS__ )

#define ecs_unit_prefix(world, ...)\
    ecs_unit_prefix_init(world, &(ecs_unit_prefix_desc_t) __VA_ARGS__ )

#define ecs_quantity(world, ...)\
    ecs_quantity_init(world, &(ecs_entity_desc_t) __VA_ARGS__ )

/* Module import */
FLECS_API
void FlecsMetaImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_EXPR
#ifdef FLECS_NO_EXPR
#error "FLECS_NO_EXPR failed: EXPR is required by other addons"
#endif
/**
 * @file addons/expr.h
 * @brief Flecs expression parser addon.
 *
 * Parse expression strings into component values. The notation is similar to
 * JSON but with a smaller footprint, native support for (large) integer types,
 * character types, enumerations, bitmasks and entity identifiers.
 * 
 * Examples:
 * 
 * Member names:
 *   {x: 10, y: 20}
 * 
 * No member names (uses member ordering):
 *   {10, 20}
 * 
 * Enum values:
 *   {color: Red}
 * 
 * Bitmask values:
 *   {toppings: Lettuce|Tomato}
 * 
 * Collections:
 *   {points: [10, 20, 30]}
 * 
 * Nested objects:
 *   {start: {x: 10, y: 20}, stop: {x: 30, y: 40}}
 * 
 */

#ifdef FLECS_EXPR

#ifndef FLECS_META
#define FLECS_META
#endif

#ifndef FLECS_PARSER
#define FLECS_PARSER
#endif

#ifndef FLECS_EXPR_H
#define FLECS_EXPR_H


/**
 * @defgroup c_addons_expr Expr
 * @brief Serialize/deserialize values to string.
 * 
 * \ingroup c_addons
 * @{
 */

/** Write an escaped character.
 * Write a character to an output string, insert escape character if necessary.
 * 
 * @param out The string to write the character to.
 * @param in The input character.
 * @param delimiter The delimiter used (for example '"')
 * @return Pointer to the character after the last one written.
 */
FLECS_API
char* ecs_chresc(
    char *out, 
    char in, 
    char delimiter);

/** Parse an escaped character.
 * Parse a character with a potential escape sequence.
 * 
 * @param in Pointer to character in input string.
 * @param out Output string.
 * @return Pointer to the character after the last one read.
 */ 
const char* ecs_chrparse(
    const char *in, 
    char *out);

/** Write an escaped string.
 * Write an input string to an output string, escape characters where necessary.
 * To determine the size of the output string, call the operation with a NULL
 * argument for 'out', and use the returned size to allocate a string that is
 * large enough.
 * 
 * @param out Pointer to output string (must be).
 * @param size Maximum number of characters written to output.
 * @param delimiter The delimiter used (for example '"').
 * @param in The input string.
 * @return The number of characters that (would) have been written.
 */
FLECS_API
ecs_size_t ecs_stresc(
    char *out, 
    ecs_size_t size, 
    char delimiter, 
    const char *in);

/** Return escaped string.
 * Return escaped version of input string. Same as ecs_stresc, but returns an
 * allocated string of the right size.
 * 
 * @param delimiter The delimiter used (for example '"').
 * @param in The input string.
 * @return Escaped string.
 */
FLECS_API
char* ecs_astresc(
    char delimiter, 
    const char *in);

/** Storage for parser variables. Variables make it possible to parameterize
 * expression strings, and are referenced with the $ operator (e.g. $var). */
typedef struct ecs_expr_var_t {
    char *name;
    ecs_value_t value;
    bool owned; /* Set to false if ecs_vars_t should not take ownership of var */
} ecs_expr_var_t;

typedef struct ecs_expr_var_scope_t {
    ecs_hashmap_t var_index;
    ecs_vec_t vars;
    struct ecs_expr_var_scope_t *parent;
} ecs_expr_var_scope_t;

typedef struct ecs_vars_t {
    ecs_world_t *world;
    ecs_expr_var_scope_t root;
    ecs_expr_var_scope_t *cur;
} ecs_vars_t;

/** Init variable storage */
FLECS_API
void ecs_vars_init(
    ecs_world_t *world,
    ecs_vars_t *vars);

/** Cleanup variable storage */
FLECS_API
void ecs_vars_fini(
    ecs_vars_t *vars);

/** Push variable scope */
FLECS_API
void ecs_vars_push(
    ecs_vars_t *vars);

/** Pop variable scope */
FLECS_API
int ecs_vars_pop(
    ecs_vars_t *vars);

/** Declare variable in current scope */
FLECS_API
ecs_expr_var_t* ecs_vars_declare(
    ecs_vars_t *vars,
    const char *name,
    ecs_entity_t type);

/** Declare variable in current scope from value.
 * This operation takes ownership of the value. The value pointer must be 
 * allocated with ecs_value_new.
 */
FLECS_API
ecs_expr_var_t* ecs_vars_declare_w_value(
    ecs_vars_t *vars,
    const char *name,
    ecs_value_t *value);

/** Lookup variable in scope and parent scopes */
FLECS_API
ecs_expr_var_t* ecs_vars_lookup(
    const ecs_vars_t *vars,
    const char *name);

/** Used with ecs_parse_expr. */
typedef struct ecs_parse_expr_desc_t {
    const char *name;
    const char *expr;
    ecs_entity_t (*lookup_action)(
        const ecs_world_t*, 
        const char *value, 
        void *ctx);
    void *lookup_ctx;
    ecs_vars_t *vars;
} ecs_parse_expr_desc_t;

/** Parse expression into value.
 * This operation parses a flecs expression into the provided pointer. The
 * memory pointed to must be large enough to contain a value of the used type.
 * 
 * If no type and pointer are provided for the value argument, the operation 
 * will discover the type from the expression and allocate storage for the 
 * value. The allocated value must be freed with ecs_value_free.
 * 
 * @param world The world.
 * @param ptr The pointer to the expression to parse.
 * @param value The value containing type & pointer to write to.
 * @param desc Configuration parameters for deserializer.
 * @return Pointer to the character after the last one read, or NULL if failed.
 */
FLECS_API
const char* ecs_parse_expr(
    ecs_world_t *world,
    const char *ptr,
    ecs_value_t *value,
    const ecs_parse_expr_desc_t *desc);

/** Serialize value into expression string.
 * This operation serializes a value of the provided type to a string. The 
 * memory pointed to must be large enough to contain a value of the used type.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @return String with expression, or NULL if failed.
 */
FLECS_API
char* ecs_ptr_to_expr(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data);

/** Serialize value into expression buffer.
 * Same as ecs_ptr_to_expr, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @param buf The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_ptr_to_expr_buf(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data,
    ecs_strbuf_t *buf);

/** Similar as ecs_ptr_to_expr, but serializes values to string. 
 * Whereas the output of ecs_ptr_to_expr is a valid expression, the output of
 * ecs_ptr_to_str is a string representation of the value. In most cases the
 * output of the two operations is the same, but there are some differences:
 * - Strings are not quoted
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @return String with result, or NULL if failed.
 */
FLECS_API
char* ecs_ptr_to_str(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data);

/** Serialize value into string buffer.
 * Same as ecs_ptr_to_str, but serializes to an ecs_strbuf_t instance.
 * 
 * @param world The world.
 * @param type The type of the value to serialize.
 * @param data The value to serialize.
 * @param buf The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_ptr_to_str_buf(
    const ecs_world_t *world,
    ecs_entity_t type,
    const void *data,
    ecs_strbuf_t *buf);

/** Serialize primitive value into string buffer.
 * Serializes a primitive value to an ecs_strbuf_t instance. This operation can
 * be reused by other serializers to avoid having to write boilerplate code that
 * serializes primitive values to a string.
 * 
 * @param world The world.
 * @param kind The kind of primitive value.
 * @param data The value ot serialize
 * @param buf The strbuf to append the string to.
 * @return Zero if success, non-zero if failed.
 */
FLECS_API
int ecs_primitive_to_expr_buf(
    const ecs_world_t *world,
    ecs_primitive_kind_t kind,
    const void *data, 
    ecs_strbuf_t *buf);

/** Parse expression token.
 * Expression tokens can contain more characters (such as '|') than tokens
 * parsed by the query (term) parser.
 * 
 * @param name The name of the expression (used for debug logs).
 * @param expr The full expression (used for debug logs).
 * @param ptr The pointer to the expression to parse.
 * @param token The buffer to write to (must have size ECS_MAX_TOKEN_SIZE)
 * @return Pointer to the character after the last one read, or NULL if failed.
 */
FLECS_API
const char *ecs_parse_expr_token(
    const char *name,
    const char *expr,
    const char *ptr,
    char *token);

/** Evaluate interpolated expressions in string.
 * This operation evaluates expressions in a string, and replaces them with
 * their evaluated result. Supported expression formats are:
 *  - $variable_name
 *  - {expression}
 * 
 * The $, { and } characters can be escaped with a backslash (\).
 * 
 * @param world The world.
 * @param str The string to evaluate.
 * @param vars The variables to use for evaluation.
 */
FLECS_API
char* ecs_interpolate_string(
    ecs_world_t *world,
    const char *str,
    const ecs_vars_t *vars);

/** Convert iterator to vars 
 * This operation converts an iterator to a variable array. This allows for
 * using iterator results in expressions. The operation only converts a 
 * single result at a time, and does not progress the iterator.
 * 
 * Iterator fields with data will be made available as variables with as name
 * the field index (e.g. "$1"). The operation does not check if reflection data
 * is registered for a field type. If no reflection data is registered for the
 * type, using the field variable in expressions will fail.
 * 
 * Field variables will only contain single elements, even if the iterator 
 * returns component arrays. The offset parameter can be used to specify which
 * element in the component arrays to return. The offset parameter must be
 * smaller than it->count.
 * 
 * The operation will create a variable for query variables that contain a
 * single entity.
 * 
 * The operation will attempt to use existing variables. If a variable does not
 * yet exist, the operation will create it. If an existing variable exists with
 * a mismatching type, the operation will fail.
 * 
 * Accessing variables after progressing the iterator or after the iterator is
 * destroyed will result in undefined behavior.
 * 
 * If vars contains a variable that is not present in the iterator, the variable
 * will not be modified.
 * 
 * @param it The iterator to convert to variables.
 * @param vars The variables to write to.
 * @param offset The offset to the current element.
 */
FLECS_API
void ecs_iter_to_vars(
    const ecs_iter_t *it,
    ecs_vars_t *vars,
    int offset);

/** @} */


#endif

#endif

#endif

#ifdef FLECS_META_C
#ifdef FLECS_NO_META_C
#error "FLECS_NO_META_C failed: META_C is required by other addons"
#endif
/**
 * @file addons/meta_c.h
 * @brief Utility macros for populating reflection data in C.
 */

#ifdef FLECS_META_C

/**
 * @defgroup c_addons_meta_c Meta Utilities
 * @brief Macro utilities to automatically insert reflection data.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_META
#define FLECS_META
#endif

#ifndef FLECS_PARSER
#define FLECS_PARSER
#endif

#ifndef FLECS_META_C_H
#define FLECS_META_C_H


/* Macro that controls behavior of API. Usually set in module header. When the
 * macro is not defined, it defaults to IMPL. */

/* Define variables used by reflection utilities. This should only be defined
 * by the module itself, not by the code importing the module */
/* #define ECS_META_IMPL IMPL */

/* Don't define variables used by reflection utilities but still declare the
 * variable for the component id. This enables the reflection utilities to be
 * used for global component variables, even if no reflection is used. */
/* #define ECS_META_IMPL DECLARE */

/* Don't define variables used by reflection utilities. This generates an extern
 * variable for the component identifier. */
/* #define ECS_META_IMPL EXTERN */

/** Declare component with descriptor */
#define ECS_META_COMPONENT(world, name)\
    ECS_COMPONENT_DEFINE(world, name);\
    ecs_meta_from_desc(world, ecs_id(name),\
        FLECS__##name##_kind, FLECS__##name##_desc)

/** ECS_STRUCT(name, body) */
#define ECS_STRUCT(name, ...)\
    ECS_META_IMPL_CALL(ECS_STRUCT_, ECS_META_IMPL, name, #__VA_ARGS__);\
    ECS_STRUCT_TYPE(name, __VA_ARGS__)

/** ECS_ENUM(name, body) */
#define ECS_ENUM(name, ...)\
    ECS_META_IMPL_CALL(ECS_ENUM_, ECS_META_IMPL, name, #__VA_ARGS__);\
    ECS_ENUM_TYPE(name, __VA_ARGS__)

/** ECS_BITMASK(name, body) */
#define ECS_BITMASK(name, ...)\
    ECS_META_IMPL_CALL(ECS_BITMASK_, ECS_META_IMPL, name, #__VA_ARGS__);\
    ECS_ENUM_TYPE(name, __VA_ARGS__)

/** Macro used to mark part of type for which no reflection data is created */
#define ECS_PRIVATE

/** Populate meta information from type descriptor. */
FLECS_API
int ecs_meta_from_desc(
    ecs_world_t *world,
    ecs_entity_t component,
    ecs_type_kind_t kind,
    const char *desc);


/* Private API */

/* Utilities to switch between IMPL, DECLARE and EXTERN variants */
#define ECS_META_IMPL_CALL_INNER(base, impl, name, type_desc)\
    base ## impl(name, type_desc)

#define ECS_META_IMPL_CALL(base, impl, name, type_desc)\
    ECS_META_IMPL_CALL_INNER(base, impl, name, type_desc)

/* ECS_STRUCT implementation */
#define ECS_STRUCT_TYPE(name, ...)\
    typedef struct __VA_ARGS__ name

#define ECS_STRUCT_ECS_META_IMPL ECS_STRUCT_IMPL

#define ECS_STRUCT_IMPL(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name);\
    static const char *FLECS__##name##_desc = type_desc;\
    static ecs_type_kind_t FLECS__##name##_kind = EcsStructType;\
    ECS_COMPONENT_DECLARE(name) = 0

#define ECS_STRUCT_DECLARE(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name);\
    ECS_COMPONENT_DECLARE(name) = 0

#define ECS_STRUCT_EXTERN(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name)


/* ECS_ENUM implementation */
#define ECS_ENUM_TYPE(name, ...)\
    typedef enum __VA_ARGS__ name

#define ECS_ENUM_ECS_META_IMPL ECS_ENUM_IMPL

#define ECS_ENUM_IMPL(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name);\
    static const char *FLECS__##name##_desc = type_desc;\
    static ecs_type_kind_t FLECS__##name##_kind = EcsEnumType;\
    ECS_COMPONENT_DECLARE(name) = 0

#define ECS_ENUM_DECLARE(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name);\
    ECS_COMPONENT_DECLARE(name) = 0

#define ECS_ENUM_EXTERN(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name)


/* ECS_BITMASK implementation */
#define ECS_BITMASK_TYPE(name, ...)\
    typedef enum __VA_ARGS__ name

#define ECS_BITMASK_ECS_META_IMPL ECS_BITMASK_IMPL

#define ECS_BITMASK_IMPL(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name);\
    static const char *FLECS__##name##_desc = type_desc;\
    static ecs_type_kind_t FLECS__##name##_kind = EcsBitmaskType;\
    ECS_COMPONENT_DECLARE(name) = 0

#define ECS_BITMASK_DECLARE(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name);\
    ECS_COMPONENT_DECLARE(name) = 0

#define ECS_BITMASK_EXTERN(name, type_desc)\
    extern ECS_COMPONENT_DECLARE(name)


#endif // FLECS_META_C_H

/** @} */

#endif // FLECS_META_C

#endif

#ifdef FLECS_PLECS
#ifdef FLECS_NO_PLECS
#error "FLECS_NO_PLECS failed: PLECS is required by other addons"
#endif
/**
 * @file addons/plecs.h
 * @brief Flecs script module.
 * 
 * For script, see examples/plecs.
 */

#ifdef FLECS_PLECS

/**
 * @defgroup c_addons_plecs Flecs script
 * @brief Data definition format for loading entity data.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_MODULE
#define FLECS_MODULE
#endif

#ifndef FLECS_PARSER
#define FLECS_PARSER
#endif

#ifndef FLECS_EXPR
#define FLECS_EXPR
#endif

#ifndef FLECS_PLECS_H
#define FLECS_PLECS_H


FLECS_API
extern ECS_COMPONENT_DECLARE(EcsScript);

/* Script component */
typedef struct EcsScript {
    ecs_vec_t using_;
    char *script;
    ecs_vec_t prop_defaults;
    ecs_world_t *world;
} EcsScript;

/** Parse plecs string.
 * This parses a plecs string and instantiates the entities in the world.
 *
 * @param world The world.
 * @param name The script name (typically the file).
 * @param str The plecs string.
 * @return Zero if success, non-zero otherwise.
 */
FLECS_API
int ecs_plecs_from_str(
    ecs_world_t *world,
    const char *name,
    const char *str);

/** Parse plecs file.
 * This parses a plecs file and instantiates the entities in the world. This
 * operation is equivalent to loading the file contents and passing it to
 * ecs_plecs_from_str.
 *
 * @param world The world.
 * @param filename The plecs file name.
 * @return Zero if success, non-zero otherwise.
 */
FLECS_API
int ecs_plecs_from_file(
    ecs_world_t *world,
    const char *filename);

/** Used with ecs_script_init */
typedef struct ecs_script_desc_t {
    ecs_entity_t entity;      /* Set to customize entity handle associated with script */
    const char *filename;     /* Set to load script from file */
    const char *str;          /* Set to parse script from string */
} ecs_script_desc_t;

/** Load managed script.
 * A managed script tracks which entities it creates, and keeps those entities
 * synchronized when the contents of the script are updated. When the script is
 * updated, entities that are no longer in the new version will be deleted.
 * 
 * This feature is experimental.
 * 
 * @param world The world.
 * @param desc Script descriptor.
 */
FLECS_API
ecs_entity_t ecs_script_init(
    ecs_world_t *world,
    const ecs_script_desc_t *desc);

#define ecs_script(world, ...)\
    ecs_script_init(world, &(ecs_script_desc_t) __VA_ARGS__)

/** Update script with new code. 
 * 
 * @param world The world.
 * @param script The script entity.
 * @param instance An assembly instance (optional).
 * @param str The script code.
 * @param vars Optional preset variables for script parameterization.
 */
FLECS_API
int ecs_script_update(
    ecs_world_t *world,
    ecs_entity_t script,
    ecs_entity_t instance,
    const char *str,
    ecs_vars_t *vars);

/** Clear all entities associated with script.
 *
 * @param world The world.
 * @param script The script entity.
 * @param instance The script instance.
 */
FLECS_API
void ecs_script_clear(
    ecs_world_t *world,
    ecs_entity_t script,
    ecs_entity_t instance);

/* Module import */
FLECS_API
void FlecsScriptImport(
    ecs_world_t *world);


#endif

/** @} */

#endif

#endif

#ifdef FLECS_RULES
#ifdef FLECS_NO_RULES
#error "FLECS_NO_RULES failed: RULES is required by other addons"
#endif
/**
 * @file addons/rules.h
 * @brief Rule query engine addon.
 * 
 * Rules are advanced queries that in addition to the capabilities of regular
 * queries and filters have the following features:
 * 
 * - query for all components of an entity (vs. all entities for a component)
 * - query for all relationship pairs of an entity
 * - support for query variables that are resolved at evaluation time
 * - automatic traversal of transitive relationships
 */

#ifdef FLECS_RULES

/**
 * @defgroup c_addons_rules Rules
 * @brief Rules are an advanced query engine for matching against entity graphs.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_RULES_H
#define FLECS_RULES_H


/** Convenience macro for rule creation */
#define ecs_rule(world, ...)\
    ecs_rule_init(world, &(ecs_filter_desc_t) __VA_ARGS__ )

/** Create a rule.
 * A rule accepts the same descriptor as a filter, but has the additional
 * ability to use query variables.
 * 
 * Query variables can be used to constrain wildcards across multiple terms to
 * the same entity. Regular ECS queries do this in a limited form, as querying
 * for Position, Velocity only returns entities that have both components.
 * 
 * Query variables expand this to constrain entities that are resolved while the
 * query is being matched. Consider a query for all entities and the mission
 * they are on:
 *   (Mission, *)
 * 
 * If an entity is on multiple missions, the wildcard will match it multiple
 * times. Now say we want to only list combat missions. Naively we could try:
 *   (Mission, *), CombatMission(*)
 * 
 * But this doesn't work, as term 1 returns entities with missions, and term 2
 * returns all combat missions for all entities. Query variables make it 
 * possible to apply CombatMission to the found mission:
 *   (Mission, $M), CombatMission($M)
 * 
 * By using the same variable ('M') we ensure that CombatMission is applied to
 * the mission found in the current result.
 * 
 * Variables can be used in each part of the term (predicate, subject, object).
 * This is a valid query:
 *   Likes($X, $Y), Likes($Y, $X)
 * 
 * This is also a valid query:
 *   _Component, Serializable(_Component)
 * 
 * In the query expression syntax, variables are prefixed with a $. When using
 * the descriptor, specify the variable kind:
 *   desc.terms[0].second = { .name = "X", .var = EcsVarIsVariable }
 * 
 * Different terms with the same variable name are automatically correlated by
 * the query engine.
 * 
 * A rule needs to be explicitly deleted with ecs_rule_fini.
 * 
 * @param world The world.
 * @param desc The descriptor (see ecs_filter_desc_t)
 * @return The rule.
 */
FLECS_API
ecs_rule_t* ecs_rule_init(
    ecs_world_t *world,
    const ecs_filter_desc_t *desc);

/** Delete a rule. 
 * 
 * @param rule The rule.
 */
FLECS_API
void ecs_rule_fini(
    ecs_rule_t *rule);

/** Obtain filter from rule.
 * This operation returns the filter with which the rule was created.
 * 
 * @param rule The rule.
 * @return The filter.
 */
FLECS_API
const ecs_filter_t* ecs_rule_get_filter(
    const ecs_rule_t *rule);

/** Return number of variables in rule.
 * 
 * @param rule The rule.
 * @return The number of variables/
 */
FLECS_API
int32_t ecs_rule_var_count(
    const ecs_rule_t *rule);

/** Find variable index.
 * This operation looks up the index of a variable in the rule. This index can
 * be used in operations like ecs_iter_set_var and ecs_iter_get_var.
 * 
 * @param rule The rule.
 * @param name The variable name.
 * @return The variable index.
 */
FLECS_API
int32_t ecs_rule_find_var(
    const ecs_rule_t *rule,
    const char *name);    

/** Get variable name.
 * This operation returns the variable name for an index.
 * 
 * @param rule The rule.
 * @param var_id The variable index.
 */
FLECS_API
const char* ecs_rule_var_name(
    const ecs_rule_t *rule,
    int32_t var_id);

/** Test if variable is an entity.
 * Internally the rule engine has entity variables and table variables. When
 * iterating through rule variables (by using ecs_rule_variable_count) only
 * the values for entity variables are accessible. This operation enables an
 * application to check if a variable is an entity variable.
 * 
 * @param rule The rule.
 * @param var_id The variable id.
 */
FLECS_API
bool ecs_rule_var_is_entity(
    const ecs_rule_t *rule,
    int32_t var_id);  

/** Iterate a rule.
 * Note that rule iterators may allocate memory, and that unless the iterator
 * is iterated until completion, it may still hold resources. When stopping
 * iteration before ecs_rule_next has returned false, use ecs_iter_fini to
 * cleanup any remaining resources.
 * 
 * @param world The world.
 * @param rule The rule.
 * @return An iterator.
 */
FLECS_API
ecs_iter_t ecs_rule_iter(
    const ecs_world_t *world,
    const ecs_rule_t *rule);

/** Progress rule iterator.
 * 
 * @param it The iterator.
 */
FLECS_API
bool ecs_rule_next(
    ecs_iter_t *it);

/** Progress instanced iterator.
 * Should not be called unless you know what you're doing :-)
 * 
 * @param it The iterator.
 */
FLECS_API
bool ecs_rule_next_instanced(
    ecs_iter_t *it);

/** Convert rule to a string.
 * This will convert the rule program to a string which can aid in debugging
 * the behavior of a rule.
 * 
 * The returned string must be freed with ecs_os_free.
 * 
 * @param rule The rule.
 * @return The string
 */
FLECS_API
char* ecs_rule_str(
    const ecs_rule_t *rule);

/** Convert rule to string with profile.
 * To use this you must set the EcsIterProfile flag on an iterator before 
 * starting iteration:
 *   it.flags |= EcsIterProfile 
 *
 * @param rule The rule.
 * @return The string
 */
FLECS_API
char* ecs_rule_str_w_profile(
    const ecs_rule_t *rule,
    const ecs_iter_t *it);

/** Populate variables from key-value string.
 * Convenience function to set rule variables from a key-value string separated
 * by comma's. The string must have the following format:
 *   var_a: value, var_b: value
 * 
 * The key-value list may optionally be enclosed in parenthesis.
 * 
 * @param rule The rule.
 * @param it The iterator for which to set the variables.
 * @param expr The key-value expression.
 */
FLECS_API
const char* ecs_rule_parse_vars(
    ecs_rule_t *rule,
    ecs_iter_t *it,
    const char *expr);


#endif // FLECS_RULES_H

/** @} */

#endif // FLECS_RULES

#endif

#ifdef FLECS_SNAPSHOT
#ifdef FLECS_NO_SNAPSHOT
#error "FLECS_NO_SNAPSHOT failed: SNAPSHOT is required by other addons"
#endif
/**
 * @file addons/snapshot.h
 * @brief Snapshot addon.
 *
 * A snapshot records the state of a world in a way so that it can be restored
 * later. Snapshots work with POD components and non-POD components, provided
 * that the appropriate lifecycle actions are registered for non-POD components.
 *
 * A snapshot is tightly coupled to a world. It is not possible to restore a
 * snapshot from world A into world B.
 */

#ifdef FLECS_SNAPSHOT

/**
 * @defgroup c_addons_snapshot Snapshot
 * @brief Save & restore world.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_SNAPSHOT_H
#define FLECS_SNAPSHOT_H


/** A snapshot stores the state of a world in a particular point in time. */
typedef struct ecs_snapshot_t ecs_snapshot_t;

/** Create a snapshot.
 * This operation makes a copy of the current state of the world.
 *
 * @param world The world to snapshot.
 * @return The snapshot.
 */
FLECS_API
ecs_snapshot_t* ecs_snapshot_take(
    ecs_world_t *world);

/** Create a filtered snapshot.
 * This operation is the same as ecs_snapshot_take, but accepts an iterator so
 * an application can control what is stored by the snapshot. 
 *
 * @param iter An iterator to the data to be stored by the snapshot.
 * @return The snapshot.
 */
FLECS_API
ecs_snapshot_t* ecs_snapshot_take_w_iter(
    ecs_iter_t *iter);

/** Restore a snapshot.
 * This operation restores the world to the state it was in when the specified
 * snapshot was taken. A snapshot can only be used once for restoring, as its
 * data replaces the data that is currently in the world.
 * This operation also resets the last issued entity handle, so any calls to
 * ecs_new may return entity ids that have been issued before restoring the 
 * snapshot.
 *
 * The world in which the snapshot is restored must be the same as the world in
 * which the snapshot is taken.
 *
 * @param world The world to restore the snapshot to.
 * @param snapshot The snapshot to restore. 
 */
FLECS_API
void ecs_snapshot_restore(
    ecs_world_t *world,
    ecs_snapshot_t *snapshot);

/** Obtain iterator to snapshot data.
 *
 * @param snapshot The snapshot to iterate over.
 * @return Iterator to snapshot data. */
FLECS_API
ecs_iter_t ecs_snapshot_iter(
    ecs_snapshot_t *snapshot);

/** Progress snapshot iterator.
 * 
 * @param iter The snapshot iterator.
 * @return True if more data is available, otherwise false.
 */
FLECS_API
bool ecs_snapshot_next(
    ecs_iter_t *iter);

/** Free snapshot resources.
 * This frees resources associated with a snapshot without restoring it.
 *
 * @param snapshot The snapshot to free. 
 */
FLECS_API
void ecs_snapshot_free(
    ecs_snapshot_t *snapshot);
    

#endif

/** @} */

#endif

#endif

#ifdef FLECS_PARSER
#ifdef FLECS_NO_PARSER
#error "FLECS_NO_PARSER failed: PARSER is required by other addons"
#endif
/**
 * @file addons/parser.h
 * @brief Parser addon.
 *
 * The parser addon parses string expressions into lists of terms, and can be
 * used to construct filters, queries and types.
 */

#ifdef FLECS_PARSER

/**
 * @defgroup c_addons_parser Parser
 * @brief Query DSL parser and parsing utilities.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_PARSER_H
#define FLECS_PARSER_H

/** Maximum number of extra arguments in term expression */
#define ECS_PARSER_MAX_ARGS (16)


/** Skip whitespace characters.
 * This function skips whitespace characters. Does not skip newlines.
 * 
 * @param ptr Pointer to (potential) whitespaces to skip.
 * @return Pointer to the next non-whitespace character.
 */
FLECS_API
const char* ecs_parse_ws(
    const char *ptr);

/** Skip whitespace and newline characters.
 * This function skips whitespace characters.
 * 
 * @param ptr Pointer to (potential) whitespaces to skip.
 * @return Pointer to the next non-whitespace character.
 */
FLECS_API
const char* ecs_parse_ws_eol(
    const char *ptr);

/** Utility function to parse an identifier */
const char* ecs_parse_identifier(
    const char *name,
    const char *expr,
    const char *ptr,
    char *token_out);

/** Parse digit.
 * This function will parse until the first non-digit character is found. The
 * provided expression must contain at least one digit character.
 * 
 * @param ptr The expression to parse.
 * @param token The output buffer.
 * @return Pointer to the first non-digit character.
 */
FLECS_API
const char* ecs_parse_digit(
    const char *ptr,
    char *token);

/** Parse a single token.
 * This function can be used as simple tokenizer by other parsers.
 * 
 * @param name of program (used for logging).
 * @param expr pointer to token to parse.
 * @param ptr pointer to first character to parse.
 * @param token_out Parsed token (buffer should be ECS_MAX_TOKEN_SIZE large)
 * @return Pointer to the next token, or NULL if error occurred.
 */
FLECS_API
const char* ecs_parse_token(
    const char *name,
    const char *expr,
    const char *ptr,
    char *token_out,
    char delim);

/** Parse term in expression.
 * This operation parses a single term in an expression and returns a pointer
 * to the next term expression.
 *
 * If the returned pointer points to the 0-terminator, the expression is fully 
 * parsed. The function would typically be called in a while loop:
 *
 * const char *ptr = expr;
 * while (ptr[0] && (ptr = ecs_parse_term(world, name, expr, ptr, &term))) { }
 *
 * The operation does not attempt to find entity ids from the names in the
 * expression. Use the ecs_term_resolve_ids function to resolve the identifiers
 * in the parsed term.
 *
 * The returned term will in most cases contain allocated resources, which
 * should freed (or used) by the application. To free the resources for a term,
 * use the ecs_term_free function.
 *
 * The parser accepts expressions in the legacy string format.
 *
 * @param world The world.
 * @param name The name of the expression (optional, improves error logs)
 * @param expr The expression to parse (optional, improves error logs)
 * @param ptr The pointer to the current term (must be in expr).
 * @param term_out Out parameter for the term.
 * @param extra_args Out array for extra args, must be of size ECS_PARSER_MAX_ARGS.
 * @return pointer to next term if successful, NULL if failed.
 */
FLECS_API
char* ecs_parse_term(
    const ecs_world_t *world,
    const char *name,
    const char *expr,
    const char *ptr,
    ecs_term_t *term_out,
    ecs_term_id_t *extra_args);


#endif // FLECS_PARSER_H

/** @} */

#endif // FLECS_PARSER

#endif

#ifdef FLECS_OS_API_IMPL
#ifdef FLECS_NO_OS_API_IMPL
#error "FLECS_NO_OS_API_IMPL failed: OS_API_IMPL is required by other addons"
#endif
/**
 * @file addons/os_api_impl.h
 * @brief Default OS API implementation.
 */

#ifdef FLECS_OS_API_IMPL

/**
 * @defgroup c_addons_os_api_impl OS API Implementation
 * @brief Default implementation for OS API interface.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_OS_API_IMPL_H
#define FLECS_OS_API_IMPL_H


FLECS_API
void ecs_set_os_api_impl(void);


#endif // FLECS_OS_API_IMPL_H

/** @} */

#endif // FLECS_OS_API_IMPL

#endif

#ifdef FLECS_MODULE
#ifdef FLECS_NO_MODULE
#error "FLECS_NO_MODULE failed: MODULE is required by other addons"
#endif
/**
 * @file addons/module.h
 * @brief Module addon.
 *
 * The module addon allows for creating and importing modules. Flecs modules 
 * enable applications to organize components and systems into reusable units of
 * code that can easily be across projects.
 */

#ifdef FLECS_MODULE

/**
 * @defgroup c_addons_module Module
 * @brief Modules organize components, systems and more in reusable units of code.
 * 
 * \ingroup c_addons
 * @{
 */

#ifndef FLECS_MODULE_H
#define FLECS_MODULE_H


/** Import a module.
 * This operation will load a modules and store the public module handles in the
 * handles_out out parameter. The module name will be used to verify if the
 * module was already loaded, in which case it won't be reimported. The name
 * will be translated from PascalCase to an entity path (pascal.case) before the
 * lookup occurs.
 *
 * Module contents will be stored as children of the module entity. This 
 * prevents modules from accidentally defining conflicting identifiers. This is
 * enforced by setting the scope before and after loading the module to the 
 * module entity id.
 *
 * A more convenient way to import a module is by using the ECS_IMPORT macro.
 *
 * @param world The world.
 * @param module The module import function.
 * @param module_name The name of the module.
 * @return The module entity.
 */
FLECS_API
ecs_entity_t ecs_import(
    ecs_world_t *world,
    ecs_module_action_t module,
    const char *module_name);

/** Same as ecs_import, but with name to scope conversion.
 * PascalCase names are automatically converted to scoped names.
 *
 * @param world The world.
 * @param module The module import function.
 * @param module_name_c The name of the module.
 * @return The module entity.
 */
FLECS_API
ecs_entity_t ecs_import_c(
    ecs_world_t *world,
    ecs_module_action_t module,
    const char *module_name_c);

/** Import a module from a library.
 * Similar to ecs_import, except that this operation will attempt to load the
 * module from a dynamic library.
 *
 * A library may contain multiple modules, which is why both a library name and
 * a module name need to be provided. If only a library name is provided, the
 * library name will be reused for the module name.
 *
 * The library will be looked up using a canonical name, which is in the same
 * form as a module, like `flecs.components.transform`. To transform this
 * identifier to a platform specific library name, the operation relies on the
 * module_to_dl callback of the os_api which the application has to override if
 * the default does not yield the correct library name.
 *
 * @param world The world.
 * @param library_name The name of the library to load.
 * @param module_name The name of the module to load.
 */
FLECS_API
ecs_entity_t ecs_import_from_library(
    ecs_world_t *world,
    const char *library_name,
    const char *module_name);

/** Register a new module. */
FLECS_API
ecs_entity_t ecs_module_init(
    ecs_world_t *world,
    const char *c_name,
    const ecs_component_desc_t *desc);

/** Define module. */
#define ECS_MODULE_DEFINE(world, id)\
    {\
        ecs_component_desc_t desc = {0};\
        desc.entity = ecs_id(id);\
        ecs_id(id) = ecs_module_init(world, #id, &desc);\
        ecs_set_scope(world, ecs_id(id));\
    }

#define ECS_MODULE(world, id)\
    ecs_entity_t ecs_id(id) = 0; ECS_MODULE_DEFINE(world, id)\
    (void)ecs_id(id)

/** Wrapper around ecs_import.
 * This macro provides a convenient way to load a module with the world. It can
 * be used like this:
 *
 * ECS_IMPORT(world, FlecsSystemsPhysics);
 */
#define ECS_IMPORT(world, id) ecs_import_c(world, id##Import, #id)


#endif

/** @} */

#endif

#endif


#endif


#endif

