# ARCHITECTURE — Documentation Technique

> Version: 0.1 (Draft)
> Projet: Site Web Personnel (Clioserv)
> Auteur: Clionalgios - Loïc GLANOIS
> Dernière mise à jour: 2026-03-06

---

# 1. Vue d'ensemble

## 1.1 Objectif du document

Ce document décrit l’architecture technique du projet :
- Organisation globale
- Composants
- Flux de requêtes
- Contraintes techniques
- Choix d’implémentation

---

# 2. Architecture Générale

## 2.1 Schéma global

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
                  │          ROUTER           │
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


Description textuelle :

- Router
- Webserver
- Système de fichiers
- Client (navigateur)
- VPS

---

## 2.2 Principes de conception

- ✅ Minimalisme
- ✅ Séparation des responsabilités
- ✅ Aucun JavaScript
- ✅ Moteur de templates maison
- ✅ Génération dynamique
- ✅ HTTPS obligatoire

---

# 3. Router

## 3.1 Rôle

- Point d’entrée unique
- Redirection HTTP → HTTPS (301)
- Distribution des requêtes vers le webserver
- Préparation au scaling futur

## 3.2 Ports

| Environnement | Router | Webserver |
|--------------|--------|------------|
| Production   | 80     | 81         |
| Développement| 8080   | 8181       |

## 3.3 Responsabilités exactes

- Validation minimale des requêtes
- Transmission interne
- Gestion des erreurs critiques

---

# 4. Webserver

## 4.1 Rôle

- Traitement des routes
- Injection templates
- Gestion multilingue
- Génération HTML dynamique
- Gestion SEO

## 4.2 Structure des routes

### Pages statiques

- /
- /whoami/
- /revival/
- /legal/
- /privacy/
- /cookies/

### Blog

- /blog/browse/{page}/{lang}/
- /blog/tag/{tag}/{page}/{lang}/
- /blog/{slug}/{lang}/

### Préférences

- /set-lang/{lang}/
- /set-theme/{theme}/

---

# 5. Structure du Contenu

## 5.1 Organisation des articles

Un article = un dossier :

/articles/
    /{slug}/
        main.html
        fr.html
        en.html
        eo.html
        uk.html
        de.html

## 5.2 Métadonnées

- slug
- statut (published / DRAFT)
- date_publication (ISO 8601)
- tags
- langues disponibles

---

# 6. Moteur de Templates

## 6.1 Fonctionnement

- Template principal
- Injection du contenu langue
- Injection des métadonnées
- Gestion du thème

## 6.2 Pipeline de rendu

1. Parsing de la route
2. Validation des paramètres
3. Chargement des métadonnées
4. Sélection langue
5. Injection dans template
6. Envoi réponse HTTP

---

# 7. Multilingue

## 7.1 Règles

- ISO 639-1
- Langue par cookie
- Canonical strict
- hreflang systématique

## 7.2 Gestion des erreurs

- Langue inexistante → 404
- Version manquante → log serveur

---

# 8. Multithèmes

## 8.1 Principe

- Cookie `theme`
- Chargement feuille CSS dynamique
- Aucune dépendance JS

## 8.2 Liste des thèmes

- Minimaliste
- Sombre
- Matrix
- Shiny

---

# 9. SEO

## 9.1 Sitemap

- /sitemap.xml
- Articles published uniquement
- lastmod dynamique
- balises hreflang

## 9.2 Robots.txt

- Blocage DRAFT-*
- Autorisation générale du site

## 9.3 Canonical

- URL avec slash final obligatoire
- Redirection 301 sinon

---

# 10. Sécurité

## 10.1 Niveau 1

- Validation stricte des entrées
- Rate limiting minimal
- Permissions système minimales
- Logs sécurisés (chmod 600)

## 10.2 Niveau 2

- Protection XSS
- Protection CSRF
- HSTS activé
- HTTPS obligatoire

---

# 11. Gestion des Logs

## 11.1 Format

- Horodatage UTC ISO 8601
- IP
- Route
- Méthode
- Code HTTP
- Séparateur spécial

## 11.2 Rétention

- 30 jours maximum
- Suppression automatique via cron

---

# 12. Performances

- Pagination obligatoire
- Optimisation images
- Limitation requêtes lourdes
- Cache éventuel (à définir)

---

# 13. Déploiement

## 13.1 Environnement

- VPS minimal
- Dual stack IPv4 / IPv6
- TLS valide
- Compte système dédié

## 13.2 Sauvegardes

- rsync vers serveur secondaire
- Dépendance GitHub pour versioning

---

# 14. Cycle de Vie du Contenu

- Vérification cohérence sitemap ↔ fichiers
- Vérification statut draft/published
- Validation au démarrage serveur

---

# 15. Points à Formaliser

- [ ] Diagramme d’architecture officiel
- [ ] Politique détaillée rate limiting
- [ ] Stratégie cache
- [ ] Tests automatisés
- [ ] Plan de reprise après incident

---

# 16. Notes & Décisions Techniques

_(Journal des décisions importantes)_

| Date | Décision | Motivation |
|------|----------|------------|
|      |          |            |

---

# 17. Roadmap Architecture

- V1
- V1.1
- V2

---

# 18. Annexes

- Diagrammes
- Schémas réseau
- Structure détaillée du code
- Convention interne supplémentaire