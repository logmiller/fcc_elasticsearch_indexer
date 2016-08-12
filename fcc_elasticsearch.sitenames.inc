<?php

/**
 * @file fcc_elasticsearch.sitename.inc
 * Elastic Search Index Doc Name
 * drush php-eval 'echo _es_get_sitename_domain_id( 'INFORUM' ) . "\n";'
 */
function _es_get_sitename_domain_id( $site_name ) {

	$domain_id = '';

	switch ( $site_name ) {

		case 'FCCNN':
			# www.fccnn.com
			$domain_id = '1';
			break;

		case 'Bemidji Pioneer':
			# www.inforum.com
			$index_name = '2';
			break;

		case 'Agweek':
			# www.agweek.com
			$domain_id = '3';
			break;

		case 'Daily Globe':
			# www.dglobe.com
			$domain_id = '4';
			break;

		case 'Detroit Lakes Online':
			# www.dl-online.com
			$domain_id = '5';
			break;

		case 'Duluth Budgeteer':
			# www.duluthbudgeteer.com
			$domain_id = '6';
			break;

		case 'Duluth News Tribune':
			# www.duluthnewstribune.com
			$domain_id = '7';
			break;

		case 'Echo Press':
			# www.echopress.com
			$domain_id = '8';
			break;

		case 'Perham Focus':
			# www.perhamfocus.com
			$domain_id = '9';
			break;

		case 'Farmington Independent':
			# www.farmingtonindependent.com
			$domain_id = '10';
			break;

		case 'Grandforks':
			# www.grandforks.com
			$domain_id = '11';
			break;

		case 'Grand Forks Herald':
			# www.grandforksherald.com
			$domain_id = '12';
			break;

		case 'Hastings Star Gazette':
			# www.hastingsstargazette.com
			$domain_id = '13';
			break;

		case 'Hudson Star Observer':
			# www.hudsonstarobserver.com
			$domain_id = '14';
			break;

		case 'INFORUM':
			# www.inforum.com
			$domain_id = '15';
			break;

		case 'Jamestown Sun':
			# www.jamestownsun.com
			$domain_id = '16';
			break;

		case 'Mitchell Advisor':
			# www.mitchelladvisor.com
			$domain_id = '17';
			break;

		case 'The Daily Republic':
			# www.mitchellrepublic.com
			$domain_id = '18';
			break;

		case 'Morris Sun Tribune':
			# www.morrissuntribune.com
			$domain_id = '19';
			break;

		case 'New Richmond News':
			# www.newrichmond-news.com
			$domain_id = '20';
			break;

		case 'Northland Outdoors':
			# www.northlandoutdoors.com
			$domain_id = '21';
			break;

		case 'Park Rapids Enterprise':
			# www.parkrapidsenterprise.com
			$domain_id = '22';
			break;

		case 'Pierce County Herald':
			# www.piercecountyherald.com
			$domain_id = '23';
			break;

		case 'Pine Journal':
			# www.pinejournal.com
			$domain_id = '24';
			break;

		case 'Prairie Business magazine':
			# www.prairiebusinessmagazine.com
			$domain_id = '25';
			break;

		case 'Republican Eagle':
			# www.republican-eagle.com
			$domain_id = '26';
			break;

		case 'River Falls Journal':
			# www.riverfallsjournal.com
			$domain_id = '27';
			break;

		case 'River Towns':
			# www.rivertowns.net
			$domain_id = '28';
			break;

		case 'Rosemount Town Pages':
			# www.rosemounttownpages.com
			$domain_id = '29';
			break;

		case 'Superior Telegram':
			# www.superiortelegram.com
			$domain_id = '30';
			break;

		case 'SWC Bulletin':
			# www.swcbulletin.com
			$domain_id = '31';
			break;

		case 'The Dickinson Press':
			# www.thedickinsonpress.com
			$domain_id = '32';
			break;

		case 'The Osakis Review':
			# www.theosakisreview.com
			$domain_id = '33';
			break;

		case 'The Bakken Today':
			# www.bakkentoday.com // NOTE: Deprecated
			$domain_id = '34';
			break;

		case 'Lake County News Chronicle':
			# www.twoharborsmn.com/www.lcnewschronicle.com
			$domain_id = '35';
			break;

		case 'Wadena PJ':
			# www.wadenapj.com
			$domain_id = '36';
			break;

		case 'West Central Tribune':
			# www.wctrib.com
			$domain_id = '37';
			break;

		case 'WDAY':
			# www.wday.com
			$domain_id = '38';
			break;

		case 'WDAZ':
			# www.wdaz.com
			$domain_id = '39';
			break;

		case 'West Fargo Pioneer':
			# www.westfargopioneer.com
			$domain_id = '40';
			break;

		case 'Woodbury Bulletin':
			# www.woodburybulletin.com
			$domain_id = '41';
			break;

		case 'Brainerd Dispatch':
			# www.brainerddispatch.com
			$domain_id = '42';
			break;

		case 'Pineandlakes Echo Journal':
			# www.pineandlakes.com
			$domain_id = '43';
			break;

		default:
			# All sites search
			$domain_id = '1';
			break;

	}
	return $domain_id;
}
