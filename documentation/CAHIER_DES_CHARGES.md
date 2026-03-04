# CAHIER DES CHARGES — Site Web Personnel

---

# 1. Présentation Générale

## 1.1 Contexte

Site web personnel comprenant :
- Un blog technique
- Un book de liens orienté “internet revival”
- Une vitrine professionnelle

Développement :
- En solo
- En C
- HTML et CSS basique
- Sans JavaScript

---

# 2. Objectifs

## 2.1 Objectifs principaux

- Servir de vitrine professionnelle
- Publier des articles (techniques, personnels, pédagogiques, loisirs)
- Centraliser projets et réalisations
- Crédibiliser un positionnement cyber et informatique

## 2.2 Objectifs secondaires

- Expérimentation technique
- Amélioration continue du moteur maison
- SEO minimal

---

# 3. Public Cible

- Amis / connaissances
- Recruteurs IT / cybersécurité
- Professionnels techniques
- Communauté CTF / OSINT
- Curieux techniques

---

# 4. Périmètre Fonctionnel (V1)

## 4.1 Pages statiques

- Accueil
- Whoami
- Revival
- Mentions légales
- Politique de confidentialité
- Pages d’erreurs dédiées : 400 / 403 / 404 / 500
- Redirections 301 / 302 basiques

## 4.2 Blog

- Liste paginée
- Barre de recherche (opérateurs inspirés Google)
- Lecture d’article
- Filtrage par tag
- Multilingue
- Flux RSS

## 4.3 Téléchargements

- CV (PDF)
- Certifications

---

# 5. Hors Périmètre (Non-objectifs)

- Pas de commentaires
- Pas de multi-auteurs
- Pas d’interface admin web
- Pas de forum
- Pas de monétisation
- Pas d’inscription utilisateur
- Pas d’analytics
- Pas de JavaScript

---

# 6. Architecture et Exigences Techniques

## 6.1 Architecture Générale

- Moteur de templates maison
- Génération dynamique
- Organisation modulaire
- HTTPS obligatoire (HTTP redirigé en 301)
- Dual stack IPv4 + IPv6
- HSTS activé

### Structure des articles

- Tous les articles dans un répertoire unique
- Un article = un sous-répertoire
- Chaque article contient :
  - Un fichier `main.html` (structure + métadonnées communes + injection template)
  - Un fichier HTML par langue (contenu + images)

### Statut des articles

Deux statuts uniquement :
- `published`
- `DRAFT`

Règles DRAFT :
- Non listé
- Meta `noindex`
- Exclu sitemap
- Accessible par URL directe
- URL mentionnant expréssement le statut `DRAFT-`

### Convention URL

Format :

```
/blog/{slug}/{langue}/
```

Règles :
- Slash final = version canonique
- Sans slash → redirection 301
- Sans langue → 404
- Ressource inexistante → 404

Slug :
- Format kebab-case

---

# 7. Multilingue

Langues disponibles :

- Français (défaut)
- Anglais
- Espéranto
- Ukrainien
- Allemand

## Règles

- Code ISO 639-1
- Énumération des langues disponibles pour chaque article au lancement serveur
- Version manquante → erreur non-fatale sur console du serveur
- Sélecteur de langue via liens inter-versions
- Affichage des articles correspondant à la langue active (cookie serveur)
- Exception possible via recherche

---

# 8. SEO (Minimal et Structuré)

- `/sitemap.xml`
- `/robots.txt`
- Structure HTML rigoureuse
- Meta tags standards
- OpenGraph
- hreflang
- Canonical strict
- Exclusion sitemap des DRAFT
- Meta `noindex` pour DRAFT
- /sitemap.xml : inclut uniquement articles published
- /robots.txt : interdit les DRAFT-*
- Balises <link rel="canonical"> pour chaque page

---

# 9. Performance

- Pagination obligatoire
- Limitation des requêtes lourdes
- Optimisation images
- Images nommées `{slug}-{index}.png`

---

# 10. Sécurité

## Niveau 1

- Validation stricte des entrées
- Contrôle des requêtes
- Compte système dédié sur VPS
- Permissions minimales
- Politique de logs :
  - Conservation max : 30 jours
  - Suppression automatisée par script
  - Accès root uniquement
  - IP non anonymisées
  - Datage des opérations
  - Contenu de la requête
  - Enregistré en fichier texte nommé en fonction du jour du lancement de la session du serveur

## Niveau 2

- Protection XSS
- Protection CSRF
- Rate limiting minimal
- HSTS

---

# 11. Exigences Légales

Cookies :
- Strictement fonctionnels
- Préférences langue / thème / support
- Aucun analytics

RGPD :
- Finalité logs : sécurité et détection d’abus
- Base légale : intérêt légitime
- Durée conservation logs : 30 jours
- Mention explicite dans :
  - Mentions légales
  - Politique de confidentialité

---

# 12. Modèle de Contenu

Encodage :
- UTF-8

Format date :
- ISO 8601

Chaque article contient :

- Header commun (métadonnées + liens langues)
- Métadonnées via balises custom + attributs
- Corps de contenu
- Références images

---

# 13. Convention de Nommage

- **Slugs** : `kebab-case`
- **Tags** : minuscules
- **Langues** : code ISO 639-1 (`fr`, `en`, `eo`, `ukr`, `de`)
- **Images** : `{slug}-{index}.png`
- **Statut articles** : `draft` / `published`
- **Fichiers articles** : un sous-dossier par article, `main.html` + fichiers par langue (`fr.html`, `en.html`…)

---

# 14. Modèle de Données Logique

## Article

```yaml
Article:
  slug: string
  statut: [draft, published]
  date_publication: ISO8601
  tags: array[string]
  langues_disponibles: array[string]
  chemins_fichiers: array[string]

Langue:
  code: ISO639-1
  nom_affiche: string
  fallback: string (optionnel)
```

---

# 15. Gestion multilingues

- URL canonical : /blog/{slug}/{lang}/
- URL sans slash final → redirection 301 vers canonical
- URL sans langue → 404
- Contenu injecté depuis fichiers lang.html dans main.html
- Affichage selon la langue stockée dans le cookie lang ou paramètre de recherche
- Liens inter-langues : bulles avec drapeaux

---

# 16. Maintenabilité

- Documentation synchronisée
- CLI d’administration
- Sauvegardes via rsync vers serveur secondaire
- Dépendance GitHub pour versionning contenu
- Changelog structuré (secondaire)

---

# 17. Contraintes

- Développement solo
- Budget nul
- VPS minimal
- Maintenance limitée

---

# 18. Cycle de vie du contenu

- Tester au démarrage du serveur, puis à intervalles régulières, que chaque objet dans sitemap ait ses fichiers correspondants et vice-versa, en tenant compte du statut des articles (draft ou published) à partir des critères de statut définis.

---

# 19. Definition of Done

- Toutes les routes fonctionnelles
- Canonical respecté
- Redirections HTTPS effectives
- Swap de langue opérationnel
- Swap de thème fonctionnel
- RSS valide
- Sitemap valide
- Pas d’erreurs serveur en navigation normale
- Documentation synchronisée
- Effacement automatique des logs au bout de 30 jours (crontab quotidien)

---

# 20. État de Complétude

| Domaine | Couverture |
|----------|------------|
| Vision stratégique | ✔ |
| Périmètre fonctionnel | ✔ |
| Non-objectifs | ✔ |
| Architecture | ✔ |
| Sécurité | Partiel |
| SEO | Basique |
| Modèle de données | Défini |
| Cycle de vie contenu | Non formalisé |
| Médias | Minimal |
| Logging détaillé | Partiel |
| Gouvernance technique | Partiel |

---