NISController.h

HTWatchDog<NISController> *_connectionWatchDog;
------------------------------------------------------------------------------------------------------------------

NISController.cpp

// ctor:
_connectionWatchDog( nullptr ), ...

// dtor:
	if( _connectionWatchDog != nullptr )
	{
		delete _connectionWatchDog;
		_connectionWatchDog = nullptr;
	}

	if( _connectionWatchDog == nullptr )
	{
		CLDelegate<NISController, HTWatchDog<NISController>::TriggerPtr> del( this, &NISController::CheckConnection );
		_connectionWatchDog = new HTWatchDog<NISController>( del, 4000 );
	}
------------------------------------------------------------------------------------------------------------------

 
