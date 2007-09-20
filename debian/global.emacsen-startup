;;==========================================================================
;; The next hack is needed for brain-dead Emacs19, which does not add
;; directories recursively to load-path
(if (string-match "^19." emacs-version)
    (setq load-path (nconc load-path
                           (list (concat "/usr/share/"
                                         (symbol-name debian-emacs-flavor)
                                         "/site-lisp/global")))))
(autoload 'gtags-mode "gtags"
          "Minor mode for browsing source code using GLOBAL" t)
;;==========================================================================

