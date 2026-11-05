---@meta
-- generated, do not edit


---@class Events
---@field SerialEventStart Event.SerialEventStart
---@field SerialEventUnitCreated Event.SerialEventUnitCreated

---@type Events
Events = nil

---@class Event.SerialEventStart : Event
---@field Add fun(self: Event.SerialEventStart, handler: fun())
---@field __call fun(self: Event.SerialEventStart)

---@class Event.SerialEventUnitCreated : Event
---@field Add fun(self: Event.SerialEventUnitCreated, handler: fun(playerID: integer, unitID: integer))
---@field __call fun(self: Event.SerialEventUnitCreated, playerID: integer, unitID: integer)
