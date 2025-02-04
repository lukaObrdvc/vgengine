;; Emacs 29.3_2

(global-set-key (kbd "M-l") 'eval-last-sexp) 
(global-set-key (kbd "M-h") 'describe-function)
(global-set-key (kbd "M-H") 'describe-variable)
;;C-h k to see command for key sequence...
;;hotkey for opening this init.el in home/.emacs.d directory...

(global-set-key (kbd "M-s") 'save-buffer) 
(global-set-key (kbd "M-f") 'find-file)
(global-set-key (kbd "M-b") 'switch-to-buffer) 
(global-set-key (kbd "M-K") 'kill-buffer) 
(global-set-key (kbd "M-g") 'goto-line)
(global-set-key (kbd "<tab>") 'dabbrev-expand)
(global-set-key (kbd "M-w") 'other-window)
(global-set-key (kbd "M-o") 'query-replace)

(global-set-key (kbd "S-<tab>") 'indent-for-tab-command)
(global-set-key (kbd "C-<tab>") 'indent-region)

(global-set-key (kbd "M-k") 'delete-other-windows)
(global-set-key (kbd "M-r") (kbd "C-u C-SPC")) ;; jump to previous mark
(global-set-key (kbd "M-n") 'next-error)
(global-set-key (kbd "M-p") 'previous-error)
(global-set-key (kbd "C-M-f") 'project-find-file)

;;rectangle-mark-mode
;;zoomin/zoomout

(setq inhibit-splash-screen t)
(setq visible-bell t)

(setq-default tab-width 4)
(setq-default indent-tabs-mode nil)
(setq-default c-basic-offset 4)

(add-hook 'window-setup-hook 'toggle-frame-maximized t)
(menu-bar-mode -1)
(tool-bar-mode -1)			
(cua-mode 1)
(abbrev-mode 1)
(global-display-line-numbers-mode 0)
(global-hl-line-mode 1)
(column-number-mode 0)
(scroll-bar-mode 0)
;;(setq-default left-margin-width 1)
;;(set-window-margins nil 1)


(setq dabbrev-case-replace t)
(setq dabbrev-case-fold-search t)
(setq dabbrev-upcase-means-case-search t)


(require 'ido)
(ido-mode t)
(ido-everywhere t)
(fido-mode t)
	
(add-to-list 'custom-theme-load-path "~/.emacs.d/themes")
;;(load-theme 'naysayer t)
(load-theme 'material t) ;; 536b75
;;(load-theme 'vscode-dark-plus t)
;;(load-theme 'noctilux t)

(set-face-attribute 'default nil :font "Hack-12")
;;(set-face-attribute 'default nil :font "Hack-30")

(setq undo-limit 100000)
(setq undo-strong-limit 200000)

(setq redisplay-dont-pause t
  scroll-margin 10
  scroll-step 1
  scroll-conservatively 10000
  scroll-preserve-screen-position 1)

(defun luka-grep (arg1)
  (interactive "sgrep findstr: ")
  (if (= (count-windows) 1) (split-window-right))
  (cd "w:/cardcrafter")
  (grep (format "findstr -s -n -i -r \"%s\" *.h *.cpp" arg1))
  (cd (file-name-directory buffer-file-name))
  )
;; how to ignore .cpp~ and stuff??

(global-set-key (kbd "M-+") 'luka-grep)

(defun luka-compile ()
  (interactive)
  (if (= (count-windows) 1) (split-window-right))
  (cd "w:/cardcrafter/code")
  (compile "build")
  (cd (file-name-directory buffer-file-name))
  )

(global-set-key (kbd "M-m") 'luka-compile)

(defun luka-bigmove-up ()
  (interactive)
  (previous-line)
  (previous-line)
  (previous-line)
  (previous-line)
  (previous-line)
  )

(global-set-key (kbd "C-<up>") 'luka-bigmove-up)

(defun luka-bigmove-down ()
  (interactive)
  (next-line)
  (next-line)
  (next-line)
  (next-line)
  (next-line)
  )

(global-set-key (kbd "C-<down>") 'luka-bigmove-down)


(defun luka-hugemove-up ()
  (interactive)
  (luka-bigmove-up)
  (luka-bigmove-up)
  (luka-bigmove-up)
  (luka-bigmove-up)
  )

(global-set-key (kbd "C-S-<up>") 'luka-hugemove-up)

(defun luka-hugemove-down ()
  (interactive)
  (luka-bigmove-down)
  (luka-bigmove-down)
  (luka-bigmove-down)
  (luka-bigmove-down)
  )

(global-set-key (kbd "C-S-<down>") 'luka-hugemove-down)


(defun _splitr ()
  (interactive)
  (split-window-right)
  )

;;(global-set-key (kbd "M-$") 'scroll-down)
;;(global-set-key (kbd "M-%") 'scroll-up) 

;; kill-buffer-and-window
;; split-window-right
;; delete-horizontal-space
;; delete-rectangle
;; delete-file
;; search C-s (C-r previous, C-s next)
;; replace(in region too) M-o (y/n)
;; quit C-g
;; minibuffer M-x
;; cua...
;; end of line C-e [end]
;; beginning of line C-a [home]
;; delete char in front C-d
;; delete word in front M-d
;; vertical move by page PGUP/PGDOWN
;; move horizontally by word C-leftarrow/C-rightarrow
;; delete line in front C-k
;; delete line C-S-k
;; new line in front C-o
;; go to definition M-. [dod search reference...]
;; begin/end of document M-> / M-<
;; comment-uncomment region M-;
;; comment-uncomment line C-x C-;

;; how to get buffers for *Buffers*, *File* and similar .... ->:
;; open dired mode(buffer) with C-x d / C-x C-j
;; list-packages ... package manager ...
;; custom-file ...
;; simpc-mode; because c-mode is slow,bad etc..... load it with load-file (path from home .emacs.d) ...
;; magit, org-mode ...
;; ido-complete-everywhere, smex (enables ido for M-x too,.....)
;; duplicate-line
;; load-path ...?


;;(setq truncate-partial-width-windows nil)
;;(setq-default truncate-lines t)
;;(setq scroll-step 3)
;;(setq grep-command "findstr -s -n -i -l str *.h *.cpp")
;;(grep-apply-setting 'grep-command '("findstr -s -n -i -r str *.h *.cpp"))
;;(setq vc-git-grep-template ())      vc grep string stuff...
;;(setq compile-command "build")
;;defcustom; what-line :type 'integer nildocumentation ......
;;(global-set-key (kbd "M-m") (kbd "M-x recompile"))
;;(completion-styles '(partial-completion substring initials flex))
;;(require 'cc-mode)
;;(defvaralias 'c-basic-offset 'tab-width)

;;(setq-default standard-indent 4)
;;(setq-default c-tab-always-indent t)
;;(setq-default c-mode-indent-offset 4)
;;(setq indent-line-function 'insert-tab) 
;;(setq tab-stop-list (number-sequence 4 200 4))

