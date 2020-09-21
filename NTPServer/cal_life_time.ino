void callifetime(){
  
  if (month > b_month) {
    // System.out.println("month > b_month : ");
               
    l_day = a_month[b_month] - b_day;
    if ((b_year % 4 == 0) && (b_month == 2) && (b_day == 29)) 
        l_day += 1;

    for(int i = 0; i<((month - 1) - b_month); i++){
        sum_day += a_month[(b_month + 1) + i];
        // System.out.println("sum_day : " + sum_day);
    }
    l_day += day + sum_day;
    // System.out.println("l_day : " + l_day);

    l_year = year - b_year;
    // System.out.println("l_year : " + l_year);
  }
  else {
    // System.out.println("month <= b_month : ");
  
    l_day = a_month[b_month] - b_day;
    if ((b_year % 4 == 0) && (b_month == 2) && (b_day == 29)) 
        l_day += 1;
  
    for(int i = 0; i<(12 - b_month); i++){
        sum_day += a_month[(b_month + 1) + i];
        // System.out.println("sum_day : " + sum_day);
      }
    l_day += sum_day;
    sum_day = 0;
    // System.out.println("l_day : " + l_day);
  
    for(int i = 0; i < (month - 1); i++){
        sum_day += a_month[i + 1];
        // System.out.println("sum_day : " + sum_day);
      }
  
    if (month > 2 && year % 4 == 0) 
        sum_day += 1;
        
    
    if(month == b_month && day >= b_day){
        // System.out.println("month == b_month && day >= b_day ");
        l_day = (day == b_day)? 0 : (day - b_day);
        // System.out.println("l_day : " + l_day);
  
        l_year = year - b_year;
        // System.out.println("l_year : " + l_year);
      }
    else {
        l_day += day + sum_day;
        // System.out.println("l_day : " + l_day); 
  
        l_year = (year - b_year) - 1;
        // System.out.println("l_year : " + l_year);
    }

  }

  sum_day = 0;
  for (int i = 0; i < l_year; i++){
      n_year = b_year + i;
      // System.out.println("l_year : " + l_year);
      if (n_year % 4 == 0) {
          sum_day += 366;
      } else {
          sum_day += 365;
      }
  }

  // System.out.println("sum_day : " + sum_day);
  l_day += sum_day;
  // System.out.println("l_day : " + l_day);

  


  if (h > b_h) {
      // System.out.println("h > b_h ");
      l_h = h - b_h;
      // System.out.println("l_h : " + l_h);

  } else {
      // System.out.println("h <= b_h ");
      if ( h == b_h) {
          l_h = 0;
      }
      else {
          l_h = h + (24 - b_h);
      }
      // System.out.println("l_h : " + l_h);
       
  }

  if (m > b_m) {
      // System.out.println("m > b_m ");
      l_m = m - b_m;
      // System.out.println("l_m : " + l_m);

  } else {
      // System.out.println("m <= b_m ");
      if ( m == b_m) {
          l_m = 0;
      }
      else {
          l_m = m + (60 - b_m);
      }
      // System.out.println("l_m : " + l_m);
       
  }

  if (s > b_s) {
      // System.out.println("s > b_s ");
      l_s = s - b_s;
      // System.out.println("l_s : " + l_s);

  } else {
      // System.out.println("s <= b_s ");
      if ( s == b_s) {
          l_s = 0;
      }
      else {
          l_s = s + (60 - b_s);
      }
      // System.out.println("l_s : " + l_s);
       
  }

  // System.out.println("l_day : "+l_day);
  // System.out.println("l_time : "+l_h+":"+l_m+":"+l_s);
  // System.out.println("x_s : " +x_s);

  l_all_s = (l_day * 24 * 3600) + (l_h * 3600) + (l_m *60) + l_s;

  // System.out.println("l_all_s : " +l_all_s);

  l_t = l_all_s * x_s;
  // System.out.println("l_t : " +l_t);

  l_t_h = l_t/3600;
//  System.out.println("l_t_h : " + (int)l_t_h);

  l_t_m = ((l_t_h - (int)l_t_h) * 60);
//  System.out.println("l_t_m : " +(int)l_t_m);

  l_t_s = (l_t_m - (int)l_t_m) * 60;

  sum_day = 0;
//  System.out.println("l_t_s : " +(int)l_t_s);
}
