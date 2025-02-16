;; this is for lerping colors
;; (defvar A (string-to-number "331900" 16))
;; (defvar B (string-to-number "330000" 16))
;; (defvar T 0.5)
;; (format "%X" (+ (* A T) (- B (* B T))) )

;; bg:
;; 000A33 000A38 000A3D 000A47 000A5C 051C19 001E66
;; 00050A 000519 00051E
;; 05000A 000005 000007 00000A 140033 300000
;; 002633 051C19 263300 332600 !!0A3833
;; 33000C
;; 200020

;; fg:
;; black (?0.4): CCCCCC
;; orange+gray: 865320 C09973

(deftheme desert-sapphire "C/C++ theme, for when you don't want anything inhibiting you.")

;; describe-face
;; list-face-display
(defun myface (name &rest args)
  (list name `((t, args)) )
  )

;; no more than 0x60 on any color
;; mixing RED+BLUE : less than 0x10 RED

;; red bg, cyan fg: b201000 ; f44B0EE ; kEE9900 ; tBBEEEE ; l00A000

;; bg : dark blue
;; fg : yellow(big g small r), beige (big r , (med)g (small)b), green(big g),
;;      !!lime (big g, small r b), !!white(big r g b)
;; fg= (AtoC)AAEE00
;; orangish can be decent on if beige is a bit whiter on main text...

(let
    (
     (bg "#000A30") ;; 000A20
     (fg "#D0AA84")
     (keyword "#D04040") ;; AAEE00 EEAA00 EEEE00 DD8800 D02020 E04040
     (type "#40A0A0") ;; EEEEEE DDDDDD
     
     (literal "#40A040") ;; 00A0A0
     (comment "#7F7F7F") ;; sixers, seveners, eighters, 7D7D7D
     (region_bg "#334055") ;;? 003333 002A2A 334b55
     
     (caret "#AF8700") ;;? 008800 AF8700
     (hl_line "#202040") ;;? 003319 000032 000050 00004A

     (modeline "#132035") ;; 334b55
     (modeline_active "#334055") ;; 536b75
     
     (red "#AA0000")
     (green "#00AA00")
     (blue "#0000AA")
     (orange "#AA8000")
     )

  (custom-theme-set-faces `desert-sapphire
                          
                          (myface 'default :background bg :foreground fg)
                          (myface 'bold :foreground fg) ;; make this bold
                          (myface 'italic :foreground fg) ;; make this italic
                          (myface 'bold-italic :foreground fg) ;; make this both
                          (myface 'underline :inherit 'default)
                          (myface 'custom-face-tag :inherit 'default)
                          (myface 'custom-state :inherit 'default)
                          (myface 'fringe :inherit 'default)
                          (myface 'vertical-border :foreground modeline_active :background modeline_active)
                          (myface 'internal-border :foreground modeline_active :background modeline_active)

                          (myface 'font-lock-builtin-face :inherit 'default)
                          (myface 'font-lock-variable-name-face :inherit 'default)
                          (myface 'font-lock-function-name-face :inherit 'default)
                          (myface 'font-lock-comment-delimiter-face :foreground comment)
                          (myface 'font-lock-comment-face :foreground comment)
                          (myface 'font-lock-constant-face :inherit 'default)
                          (myface 'font-lock-keyword-face :foreground keyword)
                          (myface 'font-lock-preprocessor-face :foreground keyword)
                          (myface 'font-lock-string-face :foreground literal)
                          (myface 'font-lock-type-face :foreground type)
                          (myface 'font-lock-warning-face :foreground red)

                          (myface 'success :foreground green)
                          (myface 'error :foreground red)
                          (myface 'warning :foreground orange)

                          (myface 'highlight :background region_bg)
                          (myface 'hl-line :background hl_line)
                          (myface 'cursor :background caret)
                          (myface 'region :background region_bg)

                          (myface 'mode-line :background modeline_active :foreground fg)
                          (myface 'mode-line-buffer-id :foreground fg)
                          (myface 'mode-line-inactive :background modeline :foreground fg)
                          (myface 'mode-line-emphasis :background modeline :foreground fg)
                          (myface 'mode-line-highlight :background modeline :foreground fg)
                          (myface 'gui-element :background modeline :foreground fg)
                          (myface 'minibuffer-prompt :foreground type)

                          ;; wgrep
                          )
  )

(provide-theme 'desert-sapphire)



