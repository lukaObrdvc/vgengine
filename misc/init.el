;; emacs 29.3_2

(load-file "W:/vgengine/misc/snippets.el")
(load-file "W:/vgengine/misc/config.el")
(load-file "W:/vgengine/misc/info.el")

(global-set-key (kbd "M-H e") 'eval-last-sexp) 
(global-set-key (kbd "M-H d f") 'describe-function)
(global-set-key (kbd "M-H d v") 'describe-variable)
(global-set-key (kbd "M-H d k") 'describe-key)
(global-set-key (kbd "M-H s") 'where-is)

;; search C-s (C-r previous, C-s next)
;; replace(in region too) M-o (y/n)
;; quit C-g
;; minibuffer M-x
;; cua
;; end of line C-e [end]
;; beginning of line C-a [home]
;; delete char in front C-d
;; delete word in front M-d
;; vertical move by page PGUP/PGDOWN
;; begin/end of document M-> / M-<
;; move horizontally by word C-leftarrow/C-rightarrow
;; yank-from-kill-ring M-y (but what if I use a prefix argument..?)
;; delete line in front C-k
;; delete line C-S-<backspace>
;; delete-horizontal-space M-\
;; jump to definition M-.

;; IMPORTANT: make TAGS file with ctags -e -R on project root

;; struct snippet 
;; case snippet
;; ?fresult

;; declare functions from source into header
;; guard (automatic?)

(global-set-key (kbd "M-f") 'project-find-file)
(global-set-key (kbd "C-M-f") 'find-file)
(global-set-key (kbd "M-b") 'switch-to-buffer)
(global-set-key (kbd "M-w") 'other-window)
(global-set-key (kbd "M-k") 'delete-other-windows)

(global-set-key (kbd "<tab>") 'dabbrev-expand)
(global-set-key (kbd "C-<tab>") 'indent-for-tab-command)
(global-set-key (kbd "M-n") 'next-error)
(global-set-key (kbd "M-p") 'previous-error)

(global-set-key (kbd "M-r") (kbd "C-u C-SPC")) ;; jump to previous mark
(global-set-key (kbd "M-t") 'pop-global-mark)
(global-set-key (kbd "M-o") 'query-replace)
(global-set-key (kbd "M-O") 'replace-string)
(global-set-key (kbd "C-/") 'comment-line)

(global-set-key (kbd "C-<prior>") 'beginning-of-defun)
(global-set-key (kbd "C-<next>") 'end-of-defun)
(global-set-key (kbd "C-M-<up>") 'backward-up-list)
(global-set-key (kbd "C-M-<down>") 'down-list)
(global-set-key (kbd "C-M-<left>") 'backward-list)
(global-set-key (kbd "C-M-<right>") 'forward-list)

(global-set-key (kbd "M-[") 'insert-parentheses)
(global-set-key (kbd "M-]") 'delete-pair)

;; if you use this key on a non-grep buffer it will be
;; set to read-only so hit C-c C-p to remove that
(global-set-key (kbd "M-_") 'wgrep-change-to-wgrep-mode)
(global-set-key (kbd "C-M-_") 'wgrep-finish-edit)

;; dabbrev-completion
;; buffer-menu -> mark with d, then hit x
;; ido-list-directory C-x C-d
;; imenu
;; tags-search

;; insert-file??
;; delete-file
;; rename-file +refactoring
;; make-directory
;; delete-directory

;; easy creation,deletion,switching,renaming of projects

(defun _splitr ()
  (interactive)
  (split-window-right)
  )

(defun _splitd ()
  (interactive)
  (split-window-below)
  )

(defun _calc ()
  (interactive)
  (quick-calc)
  )

(defun _config ()
  (interactive)
  (find-file "c:/Users/user/.emacs.d/init.el")
  )

(defun _rectselect ()
  (interactive)
  (rectangle-mark-mode)
  )

(defun _rectinsert ()
  (interactive)
  (call-interactively 'string-insert-rectangle)
  )

(defun _devenv ()
  (interactive)
  (luka-save-all)
  (cd "w:/vgengine")
  (shell-command "devenv19")
  (cd (file-name-directory buffer-file-name))
  )

(defun luka-grep (arg1)
  (interactive "sgrep findstr: ")
  (luka-save-all)
  (if (= (count-windows) 1) (split-window-right))
  (cd "w:/vgengine")
  (grep (format "findstr -s -n -i -r \"%s\" *.h *.c" arg1))
  (cd (file-name-directory buffer-file-name))
  )
(global-set-key (kbd "M-+") 'luka-grep)

(defun luka-compile ()
  (interactive)
  (luka-save-all)
  (if (= (count-windows) 1) (split-window-right))
  (cd "w:/vgengine/code")
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


