(eval-when-compile (require 'cl))
(require 'dict-tree)
(defvar dict-latex-local-latex-AI_120291_23-tex nil "Dictionary dict-latex-local-latex-AI_120291_23-tex.")
(setq dict-latex-local-latex-AI_120291_23-tex '[cl-struct-dictree- "dict-latex-local-latex-AI_120291_23-tex" "/home/letrungkien7/Dropbox/git/ML/.predictive/dict-latex-local-latex-AI_120291_23-tex" t nil < + (lambda (new old) (dictree--cell-set-data old (+ (dictree--cell-data new) (dictree--cell-data old))) old) predictive-dict-rank-function (lambda (a b) (predictive-dict-rank-function (cons (car a) (dictree--cell-data (cdr a))) (cons (car b) (dictree--cell-data (cdr b))))) time synchronize nil nil nil nil nil 0.1 nil nil nil 0.1 nil nil nil nil predictive-auto-dict-plist-savefun nil [cl-struct-trie- [nil [cl-struct-avl-tree- [nil nil nil 0] (lambda (a b) (setq a (trie--node-split a) b (trie--node-split b)) (cond ((eq a trie--terminator) (if (eq b trie--terminator) nil t)) ((eq b trie--terminator) nil) (t (< a b))))]] < (lambda (a b) (setq a (trie--node-split a) b (trie--node-split b)) (cond ((eq a trie--terminator) (if (eq b trie--terminator) nil t)) ((eq b trie--terminator) nil) (t (< a b)))) #[(cmpfun seq) "\301!\207" [cmpfun avl-tree-create] 2] avl-tree-enter avl-tree-delete avl-tree-member avl-tree-mapc avl-tree-empty avl-tree-stack avl-tree-stack-pop avl-tree-stack-empty-p trie--avl-transform-for-print trie--avl-transform-from-read t] nil])
(trie-transform-from-read (dictree--trie dict-latex-local-latex-AI_120291_23-tex))
 (trie-map
  (lambda (key cell)
     (dictree--cell-create
   (dictree--cell-data cell)
   (dictree--cell-plist cell)))
 (dictree--trie dict-latex-local-latex-AI_120291_23-tex))
(let ((cache (make-hash-table :test 'equal))
      (trie (dictree--trie dict-latex-local-latex-AI_120291_23-tex)))
  (mapc
   (lambda (entry)
     (puthash
      (car entry)
      (dictree--cache-create
       (mapcar
        (lambda (key)
          (cons key
                (trie-member
                 trie (if (stringp key) key (car key)))))
        (dictree--cache-results (cdr entry)))
       (dictree--cache-maxnum (cdr entry)))
      cache))
   (dictree--complete-ranked-cache dict-latex-local-latex-AI_120291_23-tex))
  (setf (dictree--complete-ranked-cache dict-latex-local-latex-AI_120291_23-tex)
        cache))
(let ((cache (make-hash-table :test 'equal))
      (trie (dictree--trie dict-latex-local-latex-AI_120291_23-tex)))
  (mapc
   (lambda (entry)
     (puthash
      (car entry)
      (dictree--cache-create
       (mapcar
        (lambda (key)
          (cons key
                (trie-member
                 trie (if (stringp key) key (car key)))))
        (dictree--cache-results (cdr entry)))
       (dictree--cache-maxnum (cdr entry)))
      cache))
   (dictree--regexp-ranked-cache dict-latex-local-latex-AI_120291_23-tex))
  (setf (dictree--regexp-ranked-cache dict-latex-local-latex-AI_120291_23-tex)
        cache))
(unless (memq dict-latex-local-latex-AI_120291_23-tex dictree-loaded-list)
  (push dict-latex-local-latex-AI_120291_23-tex dictree-loaded-list))
