# GO / NO-GO — Checklist Pré-Mise en Production (VPS minimal)

## 1. Système

- [ ] OS à jour
- [ ] SSH par clé uniquement
- [ ] Root SSH désactivé
- [ ] Utilisateur dédié au service
- [ ] Firewall actif (22, 80, 443 uniquement)
- [ ] Aucun service inutile exposé

---

## 2. HTTPS

- [ ] Certificat TLS valide
- [ ] Redirection 301 HTTP → HTTPS
- [ ] HSTS activé
- [ ] TLS ≥ 1.2
- [ ] Renouvellement automatique testé

---

## 3. Serveur / Moteur C

- [ ] Validation stricte des entrées (slug, langue, page)
- [ ] Regex whitelist pour slugs
- [ ] Liste blanche ISO 639-1 pour langues
- [ ] Protection path traversal
- [ ] Gestion propre 400 / 403 / 404 / 500
- [ ] Aucune information sensible dans les erreurs
- [ ] Limite taille requêtes
- [ ] Timeout configuré

---

## 4. SEO & Routage

- [ ] Slash final canonique respecté
- [ ] Redirections cohérentes
- [ ] Sitemap valide
- [ ] Robots.txt valide
- [ ] DRAFT exclus (sitemap + noindex)
- [ ] Canonical correct
- [ ] hreflang correct
- [ ] RSS valide

---

## 5. Cookies

- [ ] Secure
- [ ] SameSite défini
- [ ] Path=/
- [ ] Durée définie
- [ ] Aucune donnée sensible
- [ ] Routes `/set-lang/` et `/set-theme/` non indexables (302)

---

## 6. Sécurité HTTP

- [ ] X-Content-Type-Options: nosniff
- [ ] X-Frame-Options: DENY
- [ ] Referrer-Policy
- [ ] Content-Security-Policy minimale
- [ ] Rate limiting actif
- [ ] Retour 429 testé

---

## 7. Logs

- [ ] Hors racine web
- [ ] Permissions restreintes
- [ ] Rotation active
- [ ] Suppression > 30 jours automatique
- [ ] Test saturation disque effectué

---

## 8. Performance

- [ ] Pagination effective
- [ ] Images optimisées
- [ ] Cache-Control défini
- [ ] Temps de réponse acceptable
- [ ] Test charge légère effectué

---

## 9. Multilingue

- [ ] Toutes langues testées
- [ ] Fallback cohérent
- [ ] URL invalide → 404
- [ ] Cookie langue fonctionnel

---

## 10. Sauvegardes

- [ ] Backup complet effectué
- [ ] Restauration testée
- [ ] Sauvegarde hors VPS

---

## 11. Spécifique C

- [ ] Compilation avec `-Wall -Wextra -Werror`
- [ ] `-fstack-protector` activé
- [ ] Test `valgrind` propre
- [ ] ASLR actif
- [ ] Pas de crash au test de fuzz léger

---

# Décision

- [ ] GO — Production autorisée
- [ ] NO-GO — Corrections requises