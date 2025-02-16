(add-to-list 'load-path "~/.emacs.d/packages/")

(require 'wgrep)
(setq wgrep-auto-save-buffer t)

(add-to-list 'custom-theme-load-path "~/.emacs.d/themes")
;;(load-theme 'material t) ;; 536b75
;;(load-theme 'naysayer t)
;; handmade hero theme?
(load-theme 'desert-sapphire t)

;;(Set-Face-Attribute 'Default Nil :Font "Courier New-11")
;;(set-face-attribute 'default nil :font "Courier New-12")
;;(set-face-attribute 'default nil :font "Office Code Pro Light-11")
(set-face-attribute 'default nil :font "Office Code Pro-12")
;;(set-face-attribute 'default nil :font "iA Writer Mono S Regular-11")

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
(blink-cursor-mode 0)

;; (setq dabbrev-case-replace t)
;; (setq dabbrev-case-fold-search t)
;; (setq dabbrev-upcase-means-case-search t)

(require 'ido)
(ido-mode t)
(ido-everywhere t)
(fido-mode t)

(setq make-backup-files nil)
(setq auto-save-default nil)

(setq undo-limit 100000)
(setq undo-strong-limit 200000)

(setq redisplay-dont-pause t)
(setq scroll-margin 10)
(setq scroll-step 1)
(setq scroll-conservatively 10000)
(setq scroll-preserve-screen-position 1)

