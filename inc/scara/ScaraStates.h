/*
 * ScaraStates.h
 *
 *  Created on: 26 Apr 2018
 *      Author: ivan-zarevich
 */

#ifndef SCARA_SCARASTATES_H_
#define SCARA_SCARASTATES_H_

#include <new>

class Scara;
class ScaraActionMessage;

#define SUBSTATE(name, base) \
    class name : public base { \
    public: \
        name(Scara& context) : base(context) {}

#define STATE(name) SUBSTATE(name, ScaraBaseState)

#define CHANGE_STATE(newState) \
    new(this) newState(context); entry()

// /////////////////////////////////////////////////////////////
// Base State
// /////////////////////////////////////////////////////////////
class ScaraBaseState {
protected:
    Scara& context;
public:
   ScaraBaseState(Scara& context) :
            context(context) {
    }
    virtual ~ScaraBaseState() = default;

    virtual void commandReceived(const ScaraActionMessage& sam){

    }

    virtual void trajectoryEnd(){

    }

    virtual void park(){

    }

    virtual void cancelExecute(){

    }

    virtual void entry() {
    }

    virtual void tick() {
    }

    virtual bool tickInit() {
    	return true;
    }
};

// /////////////////////////////////////////////////////////////
// Concrete State
// /////////////////////////////////////////////////////////////
STATE(Park)
    void tick() override;
	bool tickInit() override;
    void cancelExecute() override;
    void trajectoryEnd() override;
    void entry() override;
};

STATE(IdleScara)
    void commandReceived(const ScaraActionMessage& sam) override;
	void park() override;
	void entry() override;
};

STATE(PickCube)
	void trajectoryEnd() override;
	void tick() override;
	void cancelExecute() override;
};

STATE(PreventAttach)
	void trajectoryEnd() override;
	void tick() override;
	void cancelExecute() override;
	void entry() override;
	};

#endif /* SCARA_SCARASTATES_H_ */
