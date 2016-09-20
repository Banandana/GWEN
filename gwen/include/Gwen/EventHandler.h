#pragma once

//https://facepunch.com/threads/888841-Gwen/showthread.php?t=888841&page=4

namespace Gwen {
	class EventHandler:
		public Gwen::Event::Handler
	{
	public:
		typedef void (*Target_t)(Gwen::Controls::Base*);

		EventHandler(Gwen::Event::Caller& caller, Target_t target)
			: Gwen::Event::Handler(), _caller(caller), _target(target)
		{ caller.Add(this, &EventHandler::think); }

		~EventHandler() { _caller.RemoveHandler(this); }

	private:
		void think(Gwen::Controls::Base* pControl)
		{ if (_target) return _target(pControl); }

		Gwen::Event::Caller& _caller;
		Target_t _target;
	};
};