<?php
/**
 * Basic class for creating custom blocks
 */
class CustomBlocksBlock {
  public $debug = false;
  
  /**
   * Delta for this block
   */
	public $delta = 'custom_block_delta';
  
  /**
   * Subject for this block
   */
	public $subject = '';
  
  /**
   * Description/info for this block
   */
	public $description = 'A custom block';
  
  /**
   * Since the base class may have configure() implemented, this flag allows subclasses to ignore the configure method
   */
	public $configurable = true;
	
	public function __construct($options = array()) {
		$this->setOptions($options);
	}
	
	/**
	 * Return the content for this block
	 */
	public function content() {
		return array('#markup' => 'Content');
	}
	
	/**
	 * Get a drupal variable for the given value
	 * 
	 * @param string $variable
	 * @param mixed $default
	 */
	public function get($variable, $default = false) {
		return variable_get($variable, $default);
	}
	
	/**
	 * Set a drupal variable for the given value
	 * 
	 * @param string $variable
	 * @param mixed $value
	 */
	public function set($variable, $value) {
		return variable_set($variable, $value);
	}
	
	/**
	 * Set the options from an array
	 * 
	 * Only options with a corresponding property are accepted
	 * 
	 * @param array $options
	 */
	public function setOptions($options) {
		foreach($options as $option => $value) {
			if (isset($this->$option)) {
				$this->$option = $value;
			}
		}
	}
	
	/**
	 * Implements hook_block_view for this block
	 * 
	 * Note that this only defines the block array. The block content is created with content()
	 */
	public function view() {
		$block = array(
			'content' => $this->content()
		);
		if ($this->subject) {
			$block['subject'] = $this->subject;
		}
		return $block;
	}
}