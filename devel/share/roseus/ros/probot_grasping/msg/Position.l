;; Auto-generated. Do not edit!


(when (boundp 'probot_grasping::Position)
  (if (not (find-package "PROBOT_GRASPING"))
    (make-package "PROBOT_GRASPING"))
  (shadow 'Position (find-package "PROBOT_GRASPING")))
(unless (find-package "PROBOT_GRASPING::POSITION")
  (make-package "PROBOT_GRASPING::POSITION"))

(in-package "ROS")
;;//! \htmlinclude Position.msg.html


(defclass probot_grasping::Position
  :super ros::object
  :slots (_u _v ))

(defmethod probot_grasping::Position
  (:init
   (&key
    ((:u __u) 0)
    ((:v __v) 0)
    )
   (send-super :init)
   (setq _u (round __u))
   (setq _v (round __v))
   self)
  (:u
   (&optional __u)
   (if __u (setq _u __u)) _u)
  (:v
   (&optional __v)
   (if __v (setq _v __v)) _v)
  (:serialization-length
   ()
   (+
    ;; uint16 _u
    2
    ;; uint16 _v
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint16 _u
       (write-word _u s)
     ;; uint16 _v
       (write-word _v s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint16 _u
     (setq _u (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint16 _v
     (setq _v (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get probot_grasping::Position :md5sum-) "3a9741b956dcfa783be9a0d5a4a2446e")
(setf (get probot_grasping::Position :datatype-) "probot_grasping/Position")
(setf (get probot_grasping::Position :definition-)
      "uint16 u
uint16 v

")



(provide :probot_grasping/Position "3a9741b956dcfa783be9a0d5a4a2446e")


