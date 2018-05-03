/** ***************************************************************************
 * Eurobot 2018 - RCHH
 *
 * @file
 * \addtogroup Util
 * @{
 ******************************************************************************
 */

#ifndef UTIL_GAMETIMER_H_
#define UTIL_GAMETIMER_H_

class MainFSM;

class GameTimer {
public:
    static GameTimer& getInstance();
private:
    int seconds;
    MainFSM *fsm;
private:
    GameTimer();
public:
    GameTimer(const GameTimer&) = delete;
    GameTimer(GameTimer&&) = delete;
    GameTimer& operator= (const GameTimer&) = delete;
    GameTimer& operator= (GameTimer&&) = delete;

    void start();
    void tick();
    void setFSM(MainFSM *fsm);
private:
    void initialize();
};

#endif /* UTIL_GAMETIMER_H_ */
/** @} */
