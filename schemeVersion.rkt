#lang racket
(require srfi/41) ;provides list->stream

(define test-me
  (lambda (n)
    (begin
      (display "123456789012345678901234567890")
      (newline)
      (formatter "hollow.txt" "out.txt" n))))

(define file->stream 
  (lambda (filename)
    (let ((in-port (open-input-file filename)))
      (letrec
          ((build-input-stream
            (lambda ()
              (let ((ch (read-char in-port)))
                (if (eof-object? ch)
                    (begin
                      (close-input-port in-port)
                      empty-stream)
                    (stream-cons ch (build-input-stream)))))))
        (build-input-stream)))))

(define stream->file
  (lambda (filename str)
    (let ((ex-port (open-output-file filename #:exists 'replace)))
      (letrec
          ((build-output-file
            (lambda (str)
              (cond ((not (stream-empty? str))
                     (let ((ch (stream-first str)))
                       (begin
                         (display ch) ;Awfully useful for debugging.
                         (write-char ch ex-port)
                         (build-output-file (stream-rest str)))))
                    (else (close-output-port ex-port)))))) 
        (build-output-file str)))))

(define formatter 
  (lambda (input-filename output-filename line-length)
    (stream->file output-filename ;this dun :)
                  (right-justify line-length ;this is having a couple issues :(
                                 (insert-newlines line-length ;this dun.
                                                  (remove-extra-spaces ;this dun?
                                                   (delete-newlines ;this dun?
                                                    (file->stream input-filename)))))))) ;this dun.




(define delete-newlines ;replace all newline characters with spaces
  (lambda (str)
    (cond ((stream-empty? str) empty-stream)
          ((char=? (stream-first str) #\newline)
           (stream-cons #\space ( delete-newlines (stream-rest str))))
          (else (stream-cons (stream-first str) (delete-newlines (stream-rest str)))))))


(define remove-extra-spaces ;remove any extra spaces (more than one space in a row)
  (lambda (str)
    (cond ((stream-empty? str) empty-stream)
          ((< (stream-length str) 2) empty-stream) ;adjust this
          ((and (char=? (stream-first str) #\space)  (char=? (stream-first (stream-rest str)) #\space))
           (remove-extra-spaces (stream-rest str)))
          (else (stream-cons (stream-first str) (remove-extra-spaces (stream-rest str)))))))



(define insert-newlines ;Can I please delete the space immediately preceding the newline?
  (lambda (line-length str)
    (letrec
        ((insert 
          (lambda (str count)
            (if (stream-empty? str)
                str
                (let ((n (count-chars-to-next-space str)))
                  (if (and (< count line-length) 
                           (<= (+ n count) line-length))
                      (stream-cons
                       (stream-first str)
                       (insert (stream-rest str) (+ count 1)))
                      (stream-cons
                       #\newline
                       (insert (trim-spaces str) 0))))))))
      (insert (trim-spaces str) 0))))

(define trim-spaces 
  (lambda (str)
    (cond ((stream-empty? str) empty-stream)
          ((char=? (stream-first str) #\space)
           (trim-spaces (stream-rest str)))
          (else str))))

(define count-chars-to-next-space 
  (lambda (str)
    (letrec
        ((count-ahead
          (lambda (str count)
            (cond ((stream-empty? str) count)
                  ((char=? (stream-first str) #\space) count)
                  (else (count-ahead (stream-rest str) (+ count 1)))))))
      (count-ahead str 0))))

;--ABANDON ALL HOPE YE WHO ENTER HERE------------------------------------------------------------------------------------------------------------


(define kar
  ;Takes a ->Stream<-
  ;Returns a ->LIST<- consisting of a stream's characters up to BUT NOT INCLUDING the first \n character :)
  (lambda (str)
    (if (stream-empty? str)
        null ;returns a empty list
        (if (not (char=? (stream-first str) #\newline) )
            (list* (stream-first str) (kar (stream-rest str)))
            '()))))
            ;(list (stream-first str) #\space)))));returns a list of two elements, one of which is a space
;(list (stream-first str))))));returns a list of one element

(define kdr ; returns a ->STREAM<- consisting of everything AFTER the first \n character
  (lambda (str)
    (if (stream-empty? str)
        empty-stream ;stream
        (if (and (> (stream-length str) 1) (char=? (stream-first str) #\newline))
            (stream-rest str) ;surely a stream?
            (if (> (stream-length str) 1)
                (kdr (stream-rest str))
                str)))))

(define count-spaces
  (lambda (ls)
    (cond
      ((empty? ls) 0)
      ((char=? (car ls) #\space) (+ 1 (count-spaces (cdr ls))))
      (else(count-spaces (cdr ls))))))


(define better-fluff-line
  (lambda (ln goal)
    (if(= 0 (count-spaces ln))
       ln
       (if(not (empty? ln))
          (let
              ((local-ln (chop-back-spaces ln)))
            (if(not (empty? local-ln))
               (append (fluff-line local-ln goal) (list #\newline)) ;;append \n here!
               (display "ERROR 2")))
          (display "ERROR")))))


(define fluff-line
  (lambda (ln goal); ln is inputted line, goal is goal length for line :)
    (begin
      (display "I'm showing the inputted line for fluff-line: ")
      (display ln)
      (newline)
      (let* ( (diff (- goal (- (length ln) 1)) ) ;diff is the difference between your goal and current state, you add diff spaces.
              (spcs (count-spaces ln))
              (quot (quotient diff spcs)) ;Looks like zero?
              (rem (remainder diff spcs)) )
        ;Now, at each space, we must insert quot more spaces and rem mfore spaces too, deincrementing rem to 0.
        (begin
          (display "goal ");dbg
          (display goal);dbg
          (newline);dbg
          (display "length ln ");dbg
          (display (length ln));dbg
          (newline);dbg
          (display "diff: ");dbg
          (display diff);dbg
          (newline);dbg
          (display "quot: ") ;dbg
          (display quot) ;dbg
          (newline) ;dbg
          (chop-line ln quot rem)
          )))))

(define chop-line ;Recursively inserts spaces between words in a given line. MIGHT BE BROKEN, LOOKS SUSPICIOUS
  ;Should ->Take A List <-
  ;Should ->RETURN A LIST<-
  (lambda (ln quot rem)
    (cond((empty? ln) ln) ;should return an empty list, as long as an empty list is passed in.
         ((char=? (car ln) #\space)
          (cond ((> rem 0) (cons (car ln) (add-n-spaces (chop-line (cdr ln) quot (- rem 1)) (+ quot 1) )))
                (else (cons (car ln) (add-n-spaces (chop-line (cdr ln) quot 0) quot)))))
         (else (cons (car ln) (chop-line (cdr ln) quot rem))))))

(define add-n-spaces ;APPENDS N SPACES AT THE BEGINNING OF A LINE.
  ;I think this works because it looks good to me :)
  (lambda (ls n)
    (if (> n 0)
        (add-n-spaces (cons #\space ls) (- n 1))
        ls)))

(define chop-back-spaces ;I'm seeing lines with spaces at the END, but BEFORE the \n. SAD!
  (lambda (ls)
    (let ((ls-rev (reverse ls)))
      (if (char=? (car ls-rev) #\space)
          (chop-back-spaces (reverse (cdr ls-rev)))
          ls))))

(define right-justify ;calls fluff recursively on every line of a stream
  ;takes in a ->stream<-
  ;returns a  ->stream<-
  ;ERROR: Last character of input stream repeats twice, spaces are off
  (lambda (linelength str)
    (if (stream-empty? str)
        empty-stream
        (if (> (stream-length str) 1)
            (stream-append  (list->stream (better-fluff-line (kar str) linelength)) (right-justify linelength (kdr str)))
            (list->stream '())))))
