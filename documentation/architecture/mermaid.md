flowchart TD

A[main.c / app_run] --> B[app.c FSM]

B --> C[server.c loop]
C --> D[mg_mgr_poll]

D --> E[events_handler]
E --> F[app_dispatch EVENT_HTTP_REQUEST]

F --> G[FSM Transition]
G --> H[action_http_request]

H --> I[router_dispatch]
I --> J[renderer]
J --> K[HTTP response]

B --> L[app_context]
E --> L
H --> L
