#pragma once

class IValidator {
public:
	virtual bool validate() {
		return true; // default
	};

};