#if 0
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <vector>
#include <map>


/*!
 * FnPtr defins a pointer to a function that has a string as parameter
 */
typedef void (*FnPtr)(std::string);
typedef std::vector<FnPtr> CallbackEvent;

class Event
{
public:
    /*!
     * \name addWaiter
     * \brief Method for adding a waiter function for the SerialHandelr class.
     * \param key               message key
     * \param callbackFunction  callback function
     */
    void addWaiter(std::string key, FnPtr callbackFunction);

    /*!
     * \name checkWaiter
     * \brief Method for checking the waiter functions set the SerialHandler class and for setting callback events.
     * \param response  response transmitted
     */
    void checkWaiter(std::string response);

    /*!
     * \name deleteWaiter
     * \brief Method for deleting a waiter function for the SerialHandler class.
     * \param key               message key
     * \param callbackFunction  callback function
     */
    void deleteWaiter(std::string key, FnPtr callbackFunction); //TODO not implemented
private:
    std::map<std::string, CallbackEvent > waiters;
};

#endif // EVENT_H

#endif