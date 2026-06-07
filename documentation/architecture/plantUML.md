@startuml

actor Client

Client -> Mongoose : HTTP request
Mongoose -> events_handler : MG_EV_HTTP_MSG
events_handler -> app_dispatch : EVENT_HTTP_REQUEST
app_dispatch -> FSM : transition
FSM -> action_http_request
action_http_request -> router_dispatch
router_dispatch -> renderer
renderer -> Client : HTTP response

@enduml
