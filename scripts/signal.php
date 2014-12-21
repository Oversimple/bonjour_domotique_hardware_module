<?php
/**
 * Handle a signal
 * This class is an interface to the low level scripts
 */
class SignalProcessor {

   public function __construct() {

   }
   
   /**
    * Record a signal
    * @return a string containing the hexa representation of the signal
    */
   public function record() {
      exec('pkill event.out');
      error_log('record');
      exec("/var/www/scripts/record.out",$output,$return);
      return $output[0];
   }

   /**
    *
    * Transmit a signal
    * @var $signal a string in the hexadecimal representation of the signal
    */
   public function transmit($signal) {
      error_log('transmit '.$signal);
      exec('pkill event.out');
      exec('/var/www/scripts/transmit.out '.$signal);
   }

   /**
    *
    * Start the rf in listen mode
    * If a signal is corretly interpreted from the interface, 
    * POST variable called 'signal' is send to the specified $url
    *
    */
   public function listen($url) {
      exec('pkill event.out');
      $url = $url.'?'.$variableName.'=';
      exec('./scripts/event.out > /dev/null &');
   }
}

?>
