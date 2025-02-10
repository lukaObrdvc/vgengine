(defun luka-save-all ()
  (setq current-prefix-arg '(!))
  (call-interactively 'save-some-buffers)
  )
(run-with-timer 0 (* 1 60) 'luka-save-all)

(add-to-list 'load-path "~/.emacs.d/packages/")

(require 'wgrep)
(setq wgrep-auto-save-buffer t)

;;(require 'imenu-list)

(add-to-list 'custom-theme-load-path "~/.emacs.d/themes")
(load-theme 'material t) ;; 536b75  ;; language-specific syn.highl. ....

;;(set-face-attribute 'default nil :font "Hack-12")
(set-face-attribute 'default nil :font "Courier New-11")
;;(set-face-attribute 'default nil :font "Consolas-13")

(setq inhibit-splash-screen t)
(setq visible-bell t)
(set-default 'truncate-lines t)

(setq-default tab-width 4)
(setq-default indent-tabs-mode nil)
(setq-default c-basic-offset 4)     ;; C

(add-hook 'window-setup-hook 'toggle-frame-maximized t)
(menu-bar-mode -1)
(tool-bar-mode -1)			
(cua-mode 1)
(abbrev-mode 1)
(global-hl-line-mode 1)
(scroll-bar-mode 0)

(setq dabbrev-case-replace t)
(setq dabbrev-case-fold-search t)
(setq dabbrev-upcase-means-case-search t)

(require 'ido)
(ido-mode t)
(ido-everywhere t)
(fido-mode t)
;;(semantic-mode 1)

(setq make-backup-files nil)
;;(setq create-lockfiles nil)
(setq auto-save-default nil)

(setq undo-limit 100000)
(setq undo-strong-limit 200000)

(setq redisplay-dont-pause t)
(setq scroll-margin 10)
(setq scroll-step 1)
(setq scroll-conservatively 10000)
(setq scroll-preserve-screen-position 1)

