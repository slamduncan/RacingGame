#pragma once

class Physics
{
public:
	~Physics(void);
	static Physics* Inst();
	
protected:
	Physics(void);	//constructor is protected as it's a singleton

private:
	static Physics* physInstance;
};
