/// origin guess
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"

/// 0x08dda018
FLua::Value::Value()
{
	m_pkLuaState = NULL;
	m_iRef = LUA_NOREF;
}

/// 0x08dda0fe
FLua::Value::Value(lua_State * L, int iStackIndex)
{
	m_pkLuaState = L;
	m_iRef = LUA_NOREF;
	lua_checkstack(L, 1);
	lua_pushvalue(L, iStackIndex);
	m_iRef = luaL_ref(L, LUA_REGISTRYINDEX);
}

/// 0x08dda1b6
FLua::Value::~Value()
{
	Unref();
}

/// 0x08dda2c4
void FLua::Value::Set(lua_State * L, int iStackIndex)
{
	Unref();
	if (L == NULL)
		return;
	m_pkLuaState = L;
	lua_checkstack(L, 1);
	lua_pushvalue(L, iStackIndex);
	m_iRef = luaL_ref(L, LUA_REGISTRYINDEX);
}

/// 0x08dda45a
bool FLua::Value::Push() const
{
	if (m_pkLuaState == NULL || m_iRef == LUA_NOREF)
		return false;
	lua_checkstack(m_pkLuaState, 1);
	lua_rawgeti(m_pkLuaState, LUA_REGISTRYINDEX, m_iRef);
	return true;
}

/// 0x08ddae4c
bool FLua::Table::Push() const
{
	if (!m_kLuaVal.Push())
		return false;
	if (lua_type(m_kLuaVal.GetLuaState(), -1) == LUA_TTABLE)
		return true;
	lua_settop(m_kLuaVal.GetLuaState(), -2);
	return false;
}
