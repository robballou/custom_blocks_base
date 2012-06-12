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

/**
 * Provide a simple way to make a block that outputs a list
 * 
 * Items just need to be added to the $links variable
 */
class CustomBlocksListBlock extends CustomBlocksBlock {
  public $links = array();
  
  protected $pathCache = null;
  
  public function content($options = array()){
    $items = array();
    foreach ($this->links as $link) {
      $options = array();
      if (isset($link['attributes'])) {
        $options['attributes'] = $link['attributes'];
      }
      array_push($items, l($link['title'], $this->parseUrl($link['href']), $options));
    }
    return array(
      '#theme' => 'item_list',
      '#items' => $items
    );
  }
  
  public function getNodePath() {
    if ($this->pathCache) {
      return $this->pathCache;
    }
    if (arg(0) == 'node' && is_numeric(arg(1))) {
      $this->pathCache = drupal_lookup_path('alias', 'node/'. arg(1));
      return $this->pathCache;
    }
  }
  
  public function parseUrl($url) {
    return str_replace(
      array('%node'),
      array($this->getNodePath()),
      $url
    );
  }
}