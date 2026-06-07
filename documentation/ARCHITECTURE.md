# ARCHITECTURE — Documentation Technique

## 1. Informations générales

> Version: 0.2 (Draft)
> Projet: Site Web Personnel (Clioserv)
> Auteur: Clionalgios - Loïc GLANOIS
> Dernière mise à jour: 2026-06-07

---

## 2. Table des matières

1. [Vue d'ensemble](#3-vue-densemble)
2. [Architecture Générale](#4-architecture-générale)
3. [Architecture interne (Event-Driven FSM)](#5-architecture-interne-event-driven-fsm)
4. [Gateway](#6-gateway)
5. [Webserver](#7-webserver)
6. [Structure du Contenu](#8-structure-du-contenu)
7. [Moteur de Templates](#9-moteur-de-templates)
8. [Multilingue](#10-multilingue)
9. [Multithèmes](#11-multithèmes)
10. [SEO](#12-seo)
11. [Sécurité](#13-sécurité)
12. [Gestion des Logs](#14-gestion-des-logs)
13. [Performances](#15-performances)
14. [Déploiement](#16-déploiement)
15. [Cycle de Vie du Contenu](#17-cycle-de-vie-du-contenu)
16. [Points à Formaliser](#18-points-à-formaliser)
17. [Notes & Décisions Techniques](#19-notes--décisions-techniques)
18. [Roadmap Architecture](#20-roadmap-architecture)
19. [Annexes](#21-annexes)

---

## 3. Vue d'ensemble

### 3.1 Objectif du document

Ce document décrit l’architecture technique du projet :
- Organisation globale
- Composants
- Flux de requêtes
- Contraintes techniques
- Choix d’implémentation

---

## 4. Architecture Générale

### 4.1 Schéma global

┌───────────────────────────────────────────────────────────────┐
│                       CLIENT (Navigateur)                     │
└───────────────┬───────────────────────────────┬───────────────┘
                │                               │
                │ HTTPS (IPv4/IPv6)             │
                │ Port 443                      │
                ▼                               ▼
        ┌───────────────┐               ┌───────────────┐
        │   Load        │               │     DNS       │
        │   Balancer    │               │   (Optionnel) │
        │  (Optionnel)  │               └───────────────┘
        └───────┬───────┘                       │
                │                               │
                └───────────────┬───────────────┘
                                │
                                │
                  ┌─────────────▼─────────────┐
                  │        VPS / Serveur      │
                  └─────────────┬─────────────┘
                                │
                  ┌─────────────▼─────────────┐
                  │          GATEWAY          │
                  │  (C - Port 80/8080)       │
                  │  • Redirection HTTPS      │
                  │  • Rate Limiting          │
                  │  • Filtrage basique       │
                  └─────────────┬─────────────┘
                                │
                  ┌─────────────▼─────────────┐
                  │        WEBSERVER          │
                  │     (C - Port 81/8181)    │
                  │  • Gestion des requêtes   │
                  │  • Prise en charge de GET │
                  │  seulement                │
                  │  • Templates maison       │
                  │  • Injection contenu      │
                  └─────────────┬─────────────┘
                                │
                  ┌─────────────▼──────────────┐
                  │        FILESYSTEM          │
                  ├────────────────────────────┤
                  │ ┌────────────────────────┐ │
                  │ │      templates/        │ │
                  │ │  • header.html         │ │
                  │ │  • footer.html         │ │
                  │ │  • article_frame.html  │ │
                  │ └────────────────────────┘ │
                  │ ┌────────────────────────┐ │
                  │ │       articles/        │ │
                  │ │  • slug/               │ │
                  │ │    ├ main.html         │ │
                  │ │    ├ fr.html           │ │
                  │ │    ├ en.html           │ │
                  │ │    └ images            │ │
                  │ └────────────────────────┘ │
                  │ ┌────────────────────────┐ │
                  │ │       static/          │ │
                  │ │  • {page}/             │ │
                  │ │  • ../frame.html       │ │
                  │ │  • ../{page}.css       │ │
                  │ │  • ../{langs}.html     │ │
                  │ │  • ../{images}.png     │ │
                  │ └────────────────────────┘ │
                  │ ┌────────────────────────┐ │
                  │ │     exploitation/      │ │
                  │ │  • settings.conf       │ │
                  │ └────────────────────────┘ │
                  └─────────────┬──────────────┘
                                │
                  ┌─────────────▼──────────────┐
                  │         LOGGING            │
                  │  • Fichiers session        │
                  │ serveur par session        │
                  │ serveur                    │
                  │  • Conservation 30 jours   │
                  │  • Suppression auto        │
                  └────────────────────────────┘
                                │
                  ┌─────────────▼──────────────┐
                  │    ADMINISTRATION SSH      │
                  │  • CLI maison              │
                  │  • Gestion contenu         │
                  │  • Monitoring              │
                  └─────────────┬──────────────┘
                                │
                  ┌─────────────▼──────────────┐
                  │     SAUVEGARDE RSYNC       │
                  │   Vers serveur secondaire  │
                  └────────────────────────────┘

TODO : le bloc *static* est obsolète et ne fait pas mention du folder *content/* et de ses fichiers json de contenus multilingues. Revoir cela dans une prochaine version.


Description textuelle :

- Gateway
- Webserver
- Système de fichiers
- Client (navigateur)
- VPS

---

### 4.2 Principes de conception

- ✅ Minimalisme
- ✅ Séparation des responsabilités
- ✅ Architecture event-driven
- ✅ Machine à états (FSM)
- ✅ Aucun JavaScript
- ✅ Moteur de templates maison
- ✅ Génération dynamique
- ✅ HTTPS obligatoire

## 5. Architecture interne (Event-Driven FSM)

Le webserver repose sur une architecture pilotée par événements (event-driven),
orchestrée par une machine à états finis (FSM).

### 5.1 Principes

- Tous les événements transitent par `app_dispatch`
- La FSM décide des actions à exécuter
- Mongoose agit comme source d’événements réseau
- Le routing HTTP est une action métier, non un point d’entrée

### 5.2 Séparation des responsabilités

L’architecture repose sur trois couches :

#### IO (Mongoose)
  - Gestion réseau
  - Aucun métier

#### Décision (FSM)
  - Choix des actions
  - Gestion des états

#### Action (modules applicatifs)
  - HttpHandler
  - Rendering
  - Logs

### 5.3 Flux d'exécution

Client HTTP
→ Mongoose (mg_mgr_poll)
→ events_handler (adaptateur)
→ app_dispatch(EVENT_HTTP_REQUEST)
→ FSM (transition)
→ action_http_request
→ router_dispatch
→ génération de la réponse

### 5.4 Rôles des composants

- `app.c` :
  - FSM centrale
  - gestion des transitions et actions

- `events_handler` :
  - adaptation des événements Mongoose vers événements internes

- `server.c` :
  - boucle principale (event loop)

- `router` :
  - logique métier HTTP uniquement

- `context` :
  - état global de l’application

### 5.5 Machine à états

L'application est pilotée par une FSM (Finite State Machine).

#### 5.5.1 États principaux

- INIT
- STARTING
- RUNNING
- STOPPING

#### 5.5.2 Événements

- APP_EVENT_INIT
- APP_EVENT_START
- APP_EVENT_HTTP_REQUEST
- APP_EVENT_TICK
- APP_EVENT_STOP

#### 5.5.3 Propriétés

- Centralisation du contrôle
- Prévisibilité du comportement
- Extensibilité par ajout d’événements

#### 5.5.4 Exemple

RUNNING + HTTP_REQUEST → action_http_request

---

## 6. Gateway

### 6.1 Rôle

- Composant externe au webserver
- Point d’entrée unique
- Transmission des requêtes HTTP vers webserver
- Filtrage et sécurisation en amont
- Redirection HTTP → HTTPS (301)
- Distribution des requêtes vers le webserver
- Préparation au scaling futur (éventuellement ?)

### 6.2 Ports

| Environnement | Router | Webserver |
|--------------|--------|------------|
| Production   | 80     | 81         |
| Développement| 8080   | 8181       |

### 6.3 Responsabilités exactes

- Validation des requêtes
- Transmission interne
- Gestion des erreurs critiques (dispose potentiellement de ses propres pages 404, 500, etc ?)

---

## 7. Webserver

### 7.1 Rôle

- Exécution d'une boucle événementielle (event loop)
- Réception des événements réseau via Mongoose
- Dispatch des événements via la FSM (`app_dispatch`)
- Exécution des actions applicatives (dont le routing HTTP)
- Injection templates
- Gestion multilingue
- Génération HTML dynamique
- Gestion SEO

### 7.2 Events Handler

- Callback Mongoose
- Traduction événements réseau → événements applicatifs
- Aucune logique métier

### 7.3 HttpHandler

#### 7.3.1 Principes

- Appelé uniquement via une action FSM
- Implémente la logique métier HTTP (pages, blog, etc.)

#### 7.3.2 Pages statiques

- /{lang}/
- /{lang}/whoami/
- /{lang}/revival/
- /{lang}/legal/
- /{lang}/privacy/
- /{lang}/cookies/

#### 7.3.3 Blog

- /{lang}/blog/browse/{page}/
- /{lang}/blog/tag/{tag}/{page}/
- /{lang}/blog/{english-slug}/

#### 7.3.4 Préférences

- /set-lang/{lang}/
- /set-theme/{theme}/

---

## 8. Structure du Contenu

### 8.1 Organisation des articles

Un article = un dossier :

/articles/
    /{slug}/
        main.html
        fr.html
        en.html
        eo.html
        uk.html
        de.html

### 8.2 Métadonnées

- slug
- statut (published / DRAFT)
- date_publication (ISO 8601)
- tags
- langues disponibles

---

## 9. Moteur de Templates

### 9.1 Fonctionnement

- Template principal
- Injection du contenu langue
- Injection des métadonnées
- Gestion du thème

### 9.2 Pipeline de traitement

1. Réception événement réseau (Mongoose)
2. Traduction en événement applicatif (events_handler)
3. Dispatch via FSM (`app_dispatch`)
4. Sélection de l'action selon l'état courant
5. Exécution de l'action (ex : HTTP → HttpHandler)
6. Génération de la réponse (voir pipeline de rendu)

### 9.3 Pipeline de rendu

1. Parsing de la route
2. Validation des paramètres
3. Chargement des métadonnées
4. Sélection langue
5. Injection dans template des contenus
6. Envoi réponse HTTP

---

## 10. Multilingue

### 10.1 Règles

- ISO 639-1
- Langue par cookie
- Canonical strict
- hreflang systématique

### 10.2 Gestion des erreurs

- Langue inexistante → 404
- Version manquante → log serveur

---

## 11. Multithèmes

### 11.1 Principe

- Cookie `theme`
- Chargement feuille CSS dynamique
- Aucune dépendance JS

### 11.2 Liste des thèmes

- Minimaliste
- Sombre
- Matrix
- Shiny

---

## 12. SEO

### 12.1 Sitemap

- /sitemap.xml
- Articles published uniquement
- lastmod dynamique
- balises hreflang

### 12.2 Robots.txt

- Blocage DRAFT-*
- Autorisation générale du site

### 12.3 Canonical

- URL avec slash final obligatoire
- Redirection 301 sinon

---

## 13. Sécurité

### 13.1 Niveau 1

- Validation stricte des entrées
- Rate limiting minimal
- Permissions système minimales
- Logs sécurisés (chmod 600)

### 13.2 Niveau 2

- Protection XSS
- Protection CSRF
- HSTS activé
- HTTPS obligatoire

---

## 14. Gestion des Logs

### 14.1 Format

- Horodatage UTC ISO 8601
- IP
- Route
- Méthode
- Code HTTP
- Séparateur spécial

### 14.2 Rétention

- 30 jours maximum
- Suppression automatique via cron

---

## 15. Performances

- Pagination obligatoire
- Optimisation images
- Limitation requêtes lourdes
- Cache éventuel (à définir)

---

## 16. Déploiement

### 16.1 Environnement

- VPS minimal
- Dual stack IPv4 / IPv6
- TLS valide
- Compte système dédié

### 16.2 Sauvegardes

- rsync vers serveur secondaire
- Dépendance GitHub pour versioning

---

## 17. Cycle de Vie du Contenu

- Vérification cohérence sitemap ↔ fichiers
- Vérification statut draft/published
- Validation au démarrage serveur

---

## 18. Points à Formaliser

- [ ] Diagramme d’architecture officiel
- [ ] Politique détaillée rate limiting
- [ ] Stratégie cache
- [ ] Tests automatisés
- [ ] Plan de reprise après incident

---

## 19. Notes & Décisions Techniques

_(Journal des décisions importantes)_

| Date | Décision | Motivation |
|------|----------|------------|
|      |          |            |

---

## 20. Roadmap Architecture

- V1
- V1.1
- V2

---

## 21. Annexes

- Diagrammes
- Schémas réseau
- Structure détaillée du code
- Convention interne supplémentaire