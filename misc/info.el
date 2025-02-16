;; (set-face-attribute 'default nil :font "AverageMono-12")


;;(set-face-attribute 'default nil :font "Office Code Pro-11")

;; (set-face-attribute 'default nil :font "TeXGyreCursor-Regular-12")

;;(set-face-attribute 'default nil :font "IBM Plex Mono Regular-11")
;;(set-face-attribute 'default nil :font "IBM Plex Mono Light-11")
;;(set-face-attribute 'default nil :font "Courier Prime Sans-11")

;; (set-face-attribute 'default nil :font "Office Code Pro-11")
;; (set-face-attribute 'default nil :font "Source Code Pro Regular-11")
;; (set-face-attribute 'default nil :font "NotCourierSans-11")
;; (set-face-attribute 'default nil :font "Inconsolata-g-11")
;; (set-face-attribute 'default nil :font "FreeMono-12")


;; (set-face-attribute 'default nil :font "Fira Code Regular-10")
;; (set-face-attribute 'default nil :font "Iosevka Light-12")


;; (set-face-attribute 'default nil :font "Iosevka Slab Light-12")
;; (set-face-attribute 'default nil :font "DejaVu Sans Mono-11")
;; (set-face-attribute 'default nil :font "Ubuntu Light-12")
;; (set-face-attribute 'default nil :font "Hack-12")
;; (set-face-attribute 'default nil :font "Liberation Mono-11")
;; (set-face-attribute 'default nil :font "Consolas-12")









;; scope a region and move point above scope
;; same but unscope

;; surrounding with #if 0   #endif


;;(global-set-key (kbd "M-s") 'save-buffer)
;;(global-set-key (kbd "M-K") 'kill-buffer)

;; M-x C-M-m is useful, gives you completion of everything in emacs
;; so you can search it as well



;; how to set comment style to be // in C?

;; kill-sentence
;; ~~ select to end of line
;; new line in front C-o (C-M-O similar) (C-j) (similar stuffs with C,M,S..)
;; C-m to add line above essentially


;; buffer-menu ; ibuffer
;; moving by defuns
;; occur
;; multi-occur
;; buffer-menu-multi-occur
;; set-visited-file-name

;; dired -> is useful to call sometimes
;;? begin of line but after indent
;;? copy line
;; perchaaaaaaaaaance autocomplete
;; perchaaaaance yasnippets
;; formatting

;; you can zoomin/out with C+mousewheel

;; assume files are opened already (through project-find-file)
;; get a list of buffers with C-u C-x C-b
;; you can either get the paths, or get buffer names, then use
;; get-file-buffer then buffer-file name (but this is probably stupid)

;; you click on lines and it adds file paths like this,
;; then you obtain your own list, where you can remove paths
;; or add new ones

;; (perchance further select from this list which you want further...)

;; for every path, call occur on the file

;; you can mark with m in buffer menu, then call
;; Buffer-menu-multi-occur first for ^{
;; you obtain list of all files, you obtain all line numbers
;; you decrement line numbers, you read entire line on line numbers
;; you place all those lines per file after each other
;; this way you get all function signatures for files you want
;; you can then choose to add to the file list other files,
;; remove files, remove functions
;; (perchance add a function a new one when you add it)
;; you are able to jump to a function signature
;; you are able to declare function signatures in a header file
;; (assuming they are not inline)
;; (perchange do mark-defun on a function signature)

;; instead of occur you could move by defuns

;; why ever do this on source files



;; BRUUUUUUUUUH you need to fuck off with this complexity, procrastination,
;; basically OOP, complex premature optimization algorithms type of shize,
;; just look at what casey had and add upon that very little and
;; straightforward functionallity for which it is easy to see benefits
;; and there are no downsides.....




;; call-interactively -> appears to call as if from minibuffer
;; ' add to function to treat it as a variable ??



;; how to ignore .cpp~ and stuff??
;; I believe this is impossible with findstr





;; list-packages ... package manager ...
;; custom-file ...
;; simpc-mode; because c-mode is slow,bad etc..... load it with load-file (path from home .emacs.d) ...
;; magit, org-mode ...
;; ido-complete-everywhere, smex (enables ido for M-x too,.....)
;; load-path ...?
;; ctags...

;;(require 'cc-mode)

;;(global-display-line-numbers-mode 0)
;;(column-number-mode 0)
;;(setq-default left-margin-width 1)
;;(set-window-margins nil 1)

;; duplicate-line
