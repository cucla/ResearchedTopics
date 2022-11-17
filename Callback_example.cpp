#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <functional>
#include <unordered_set>
#include <bitset>
#include <algorithm>



enum MICROPHONE_STATUS
{
	MS_ALL_MICROPHONES_UNAVAILABLE,
	MS_MUTED,
	MS_UNMUTED,
	MS_MICROPHONE_CHANGED
};

typedef std::function<void( MICROPHONE_STATUS )> MIC_CALLBACK_SHARED;
typedef std::shared_ptr<MIC_CALLBACK_SHARED> MIC_CALLBACK;


//----------------------------------------------------------------------------------------------------
class SMMuteMonitor
{
public:
	inline static SMMuteMonitor * GetInstance();

	void RegisterCallback( MIC_CALLBACK &cb )
	{
		AccessCallbacks( [&cb] ( std::unordered_set <MIC_CALLBACK> &cblist ) { cblist.insert( cb ); } );
	}
	void UnRegisterCallback( MIC_CALLBACK cb )
	{
		AccessCallbacks( [&cb] ( std::unordered_set <MIC_CALLBACK> &cblist )
		{
			if( cblist.find( cb ) == cblist.end() ) return;
			cblist.erase( cb );
		}
		);
	}

	void EmitCurrentStatus( MICROPHONE_STATUS ms, bool force )
	{
		MICROPHONE_STATUS _activeMicStatus = ms;

		AccessCallbacks( [this]( auto &cblist )
		{
			for( MIC_CALLBACK cb : cblist )
			{
				( *cb )( _activeMicStatus );
			}
		}
		);
	}

private:
	SMMuteMonitor() {}
	virtual ~SMMuteMonitor() {}
	static SMMuteMonitor * _muteMon;

	std::unordered_set<MIC_CALLBACK> _callbacks;

	void AccessCallbacks( std::function<void( std::unordered_set<MIC_CALLBACK> & )> fnc )
	{
		fnc( _callbacks );
	}
};

SMMuteMonitor * SMMuteMonitor::_muteMon = nullptr;

inline SMMuteMonitor * SMMuteMonitor::GetInstance()
{
	if( _muteMon == nullptr )
	{
		_muteMon = new SMMuteMonitor();
	}
	return _muteMon;
}


//----------------------------------------------------------------------------------------------------
class SMAutoMuteFeature
{
public:
	SMAutoMuteFeature()
	{
		_microphoneCb = std::make_shared<MIC_CALLBACK_SHARED>( ( [this]( MICROPHONE_STATUS mc )
		{
			std::cout << " Called lambda from SMAutoMuteFeature ctor " << std::endl;

			if( mc == MICROPHONE_STATUS::MS_MUTED || mc == MICROPHONE_STATUS::MS_UNMUTED )
			{
				bool mute = ( mc == MICROPHONE_STATUS::MS_MUTED );
				//SetMute( mute, true );
			}
		} ) );
	}
	bool Activate()
	{
		SMMuteMonitor::GetInstance()->RegisterCallback( _microphoneCb );
	}
	bool Deactivate()
	{
		SMMuteMonitor::GetInstance()->UnRegisterCallback( _microphoneCb );
	}

private:
	MIC_CALLBACK _microphoneCb;
};



int main()
{



    std::cout << "\n";
}
