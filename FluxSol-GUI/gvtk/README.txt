https://godotengine.org/qa/9758/timer-node-how-to-use-it-in-code

Somewhere, like in _ready:

timer = Timer.new()
timer.connect("timeout",self,"_on_timer_timeout") 
#timeout is what says in docs, in signals
#self is who respond to the callback
#_on_timer_timeout is the callback, can have any name
add_child(timer) #to process
timer.start() #to start
Then, on the connected node (self in this case, could be the parent or any other node in the scene)

func _on_timer_timeout():
   your_timer_stuff()
   
https://godotengine.org/qa/9065/how-to-call-a-method-only-every-1-second

var _timer = null

func _ready():
    _timer = Timer.new()
    add_child(_timer)

    _timer.connect("timeout", self, "_on_Timer_timeout")
    _timer.set_wait_time(1.0)
    _timer.set_one_shot(false) # Make sure it loops
    _timer.start()


func _on_Timer_timeout():
    print("Second!")