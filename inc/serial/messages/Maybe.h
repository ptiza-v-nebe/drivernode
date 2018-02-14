/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup SerialMessages
 * @{
 ******************************************************************************
 */

#ifndef SERIAL_MESSAGES_MAYBE_H_
#define SERIAL_MESSAGES_MAYBE_H_

template<class T>
class Maybe {
public:
    static Maybe<T> EMPTY;
private:
    bool valid;
    T object;
private:
    Maybe();
public:
    Maybe(T object);

    T get();
    bool isValid();
};

template<class T>
inline Maybe<T>::Maybe() : valid(false) {
}

template<class T>
inline Maybe<T>::Maybe(T object) : valid(true), object(object) {
}

template<class T>
inline T Maybe<T>::get() {
    return object;
}

template<class T>
inline bool Maybe<T>::isValid() {
    return valid;
}

#endif /* SERIAL_MESSAGES_MAYBE_H_ */
/** @} */
