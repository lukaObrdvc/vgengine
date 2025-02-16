;; quick open scratch buffer
;; ? on command in minibuffer to show possible completions
;; f3 start macro, f4 end macro or replay last
;; or just use multiple cursors bro??

;; see casey's too

;; jblow keybindings:
;; M-n create-new-file
;; C-p move-up
;; C-n move-down
;; C-F move-right (other way around ??)
;; C-b move-left
;; C-a begin of line
;; C-e end of line
;; C-s search
;; C-d delete char in front
;; C-l center around current line
;; C-k delete to end of line (then pastes...)
;; C/M-v scroll up/down
;; C-x o ?? (popping back and forth; windows??) C-o the default
;; create window
;; C-0 ?close window all other windows? "snap back to just the buffer"
;; begin/end of file (C-. C-, ??)
;; auto-indent region
;; C-y paste
;; go to begin of code?
;; replace (without asking)
;; uses C-k to remove multiple lines and then pastes them all together??

;; emacs 29.3_2

;; IMPORTANT remove ido/fido but can autocomplete with tab (make that work???)

(load-file "W:/vgengine/misc/config.el")

(global-set-key (kbd "M-H e") 'eval-last-sexp) 
(global-set-key (kbd "M-H d f") 'describe-function)
(global-set-key (kbd "M-H d v") 'describe-variable)
(global-set-key (kbd "M-H d k") 'describe-key)
(global-set-key (kbd "M-H s") 'where-is)

;;*+ parenstuff, comment out

;; search C-s (C-r previous, C-s next)
;;* replace(in region too) M-o (y/n)
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
;;* delete line in front C-k
;; delete line C-S-<backspace>
;;* delete-horizontal-space M-\
;; jump to definition M-.

;; IMPORTANT: make TAGS file with ctags -e -R on project root

;; automatic window split and open all relevant files on startup...
;; automatic reloading of ctags (and other things like this...)

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

(defun __save-all ()
  (interactive)
  (setq current-prefix-arg '(!))
  (call-interactively 'save-some-buffers)
  )
(run-with-timer 0 30 '__save-all)

(defun __grep (arg1)
  (interactive "sgrep findstr: ")
  (__save-all)
  (if (= (count-windows) 1) (split-window-right))
  (cd "w:/vgengine")
  (grep (format "findstr -s -n -i -r \"%s\" *.h *.c" arg1))
  (cd (file-name-directory buffer-file-name))
  )
(global-set-key (kbd "M-+") '__grep)

(defun __compile ()
  (interactive)
  (__save-all)
  (if (= (count-windows) 1) (split-window-right))
  (cd "w:/vgengine/code")
  (compile "build")
  (cd (file-name-directory buffer-file-name))
  )
(global-set-key (kbd "M-m") '__compile)

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

(defun __bigmove-up ()
  (interactive)
  (previous-line)
  (previous-line)
  (previous-line)
  (previous-line)
  (previous-line)
  )
(global-set-key (kbd "C-<up>") '__bigmove-up)

(defun __bigmove-down ()
  (interactive)
  (next-line)
  (next-line)
  (next-line)
  (next-line)
  (next-line)
  )
(global-set-key (kbd "C-<down>") '__bigmove-down)

(defun __hugemove-up ()
  (interactive)
  (__bigmove-up)
  (__bigmove-up)
  (__bigmove-up)
  (previous-line)
  (previous-line)
  (previous-line)
  (previous-line)
  )
(global-set-key (kbd "C-S-<up>") '__hugemove-up)

(defun __hugemove-down ()
  (interactive)
  (__bigmove-down)
  (__bigmove-down)
  (__bigmove-down)
  (next-line)
  (next-line)
  (next-line)
  (next-line)
  )
(global-set-key (kbd "C-S-<down>") '__hugemove-down)


