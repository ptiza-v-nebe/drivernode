/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Util
 * @{
 ******************************************************************************
 */


#ifndef UTIL_ENUM_TO_STRING_H_
#define UTIL_ENUM_TO_STRING_H_

/*
 * Source: https://codereview.stackexchange.com/questions/14309/conversion-between-enum-and-string-in-c-class-header
 *
 * Usage:
 *  // Define Enum Like this
 *  enum class X {Hi, Lo};
 *  // Then you just need to define their string values.
 *  template<> char const* enumStrings<X>::data[] = {"Hi", "Lo"};
 *
 */

// This is the type that will hold all the strings.
// Each enumerate type will declare its own specialization.
// Any enum that does not have a specialization will generate a compiler error
// indicating that there is no definition of this variable (as there should be
// be no definition of a generic version).
template<typename T>
struct EnumStrings
{
    static char const* data[];
};

/**
 * Converts an enum into a string using template argument deduction.
 *
 * @param enumValue the enum value to be turned into a string
 *
 * @return the string representing that enum value
 */
template<typename T>
const char* enumToString(const T& enumValue) {
    return EnumStrings<T>::data[static_cast<int>(enumValue)];
}

// ///////////////////////////////////////////////
// Now some macro magic to generate enums
// ///////////////////////////////////////////////
#define SMARTENUM_VALUE(name) name,
#define SMARTENUM_STRING(name) #name,

/**
 * Defines an enum.
 * Usage:
 * 1) Define a Macro for the values:
 * #define MY_VALUES(m) \
 *      m(VALUE_1) \
 *      m(VALUE_2)
 *
 * 2) Call SMARTENUM_DEFINE_ENUM with name and values.
 */
#define SMARTENUM_DEFINE_ENUM(typeName, values) enum class typeName { values(SMARTENUM_VALUE) typeName##_Count, };

/**
 * Defines strings for an enum.
 * Usage:
 * 1) Define a Macro for the values:
 * #define MY_VALUES(m) \
 *      m(VALUE_1) \
 *      m(VALUE_2)
 *
 * 2) Call SMARTENUM_DEFINE_NAMES with name and values.
 */
#define SMARTENUM_DEFINE_NAMES(typeName, values) template<> const char* EnumStrings<typeName>::data [] = { values(SMARTENUM_STRING) };

#endif /* UTIL_ENUM_TO_STRING_H_ */
/** @} */
