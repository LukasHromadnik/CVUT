function result = createdata(action,varargin)
% CREATEDATA Interactive data generator.
%
% Synopsis:
%  createdata
%  createdata('finite')
%  createdata('finite',num_classes)
%  createdata('gauss')
%  createdata('gauss',num_distrib)
%
% Description:
%  createdata or createdata('finite') invokes an interactive generator 
%  of finite point sets in 2D. The generated points can be assigned 
%  into two classes (1,2). The generator saves data to a 
%  specified file. The file has the following fields:
%   X [2 x num_data] 2D vectors.
%   y [1 x num_data] Assigned labels.
%
%  createdata('finite',num_classes) when num_classes is specified
%   then the points can be assigned labels from 1 to num_classes.
%  
%  createdata('gauss') invokes an interactive generator of 
%   Gaussian distributions. A user can specify mean vector and
%   covariance matrix. The generator saves data to a file
%   having the following fields:
%    Mean [2 x ncomp] Mean vectors.
%    Cov [2 x 2 x ncomp] Covariance matrices.
%    y [1 x ncomp] Assigned labels.
%
%  createdata('gauss',num_classes) when num_classes is specified
%   then the distributions can be assigned labels from 1 to num_classes.
%  
%  The data generator is controlled by mouse:
%    Left button  ... creates a new point or Gaussian.
%    Right button ... erases the focused point or Gaussian.
%
%  In the case of generating Gaussians left button double-click makes
%  selected Gaussian focused. The covariance matrix of focused 
%  Gaussian can be modified.
%

% About: Statistical Pattern Recognition Toolbox
% (C) 1999-2003, Written by Vojtech Franc and Vaclav Hlavac 
% <a href="http://www.cvut.cz">Czech Technical University Prague</a>
% <a href="http://www.feld.cvut.cz">Faculty of Electrical Engineering</a>
% <a href="http://cmp.felk.cvut.cz">Center for Machine Perception</a>
                                                                                
% Modifications:   
% 01-may-2004, VF
% 13-Feb-2003, VF  


XCOLORS=['r','b','g','m','c','k','y'];      % colors of the ellipses
MAXCOLOR=size(XCOLORS,2);
ID_NORMAL='Infinite sets, Normal distributions';
ID_FINITE='Finite sets, Enumeration';
DX_SPACE=0.5;   % space betwean the nearest point and the left and right border
DY_SPACE=0.5;   % --//--                                  top and bottom -//-

if nargin < 1,
   action = 'finite';
end

switch lower(action)
case 'finite'
   % == We will create finite data sets ===================================
   if nargin < 2,
      K=2;
   else
      K=varargin{1};
   end
   createdata('initialize',action,K,varargin{2:nargin-1});

case 'gauss'
   % == We will create gaussian distributed sets  ===========================
   if nargin < 2,
      K=2;
   else
      K=varargin{1};
   end
   createdata('initialize',action,K,varargin{2:nargin-1});


case 'initialize'
   % == Initialize dialog window =========================================

   % get input arguments
   K=varargin{2};

   % == Figure ===========================================================
   hfigure = figure(...
      'Visible','off',...
    'NumberTitle','off', ...
       'Units','normalized', ...
      'RendererMode','manual');

   % == Axes =============================================================
   haxes1= axes(...
       'Units','normalized', ...
      'ButtonDownFcn','createdata(''click'',gcf)',...
      'Box','on', ...
      'XGrid','on', ...
      'YGrid','on', ...
      'NextPlot','add',...
      'Position',[0.1 0.1 0.65 0.85]);
   axis([-1 1 -1 1]);
   xlabel('X');
   ylabel('Y');


   % == Buttons ==========================================================
   left=0.8;
   bottom=0.05;
   height=0.05;
   width=0.15;

   % close button
   hbtclose = uicontrol( ...
      'Units','Normalized', ...
      'Callback','createdata(''close'',gcf)',...
        'ListboxTop',0, ...
        'Position',[left bottom width height], ...
        'String','Close');

   % ok button
   bottom=bottom+1*height;
   hbtok = uicontrol( ...
      'Units','Normalized', ...
      'Callback','createdata(''ok'',gcf)',...
        'ListboxTop',0, ...
      'UserData',varargin,...
      'Position',[left bottom width height], ...
        'String','OK');

   % info button
   bottom=bottom+1.5*height;
   hbtinfo = uicontrol( ...
      'Units','Normalized', ...
      'Callback','createdata(''info'',gcf)',...
        'ListboxTop',0, ...
        'Position',[left bottom width height], ...
        'String','Info');

   % load button
   bottom=bottom+1.5*height;
   hbtload = uicontrol( ...
      'Units','Normalized', ...
      'Callback','createdata(''load'',gcf)',...
        'ListboxTop',0, ...
        'Position',[left bottom width height], ...
        'String','Load');

   % save button
   file=struct('name','noname.mat','path','','pathname','noname.mat');
   bottom=bottom+1*height;
   hbtsave = uicontrol( ...
      'Units','Normalized', ...
      'Callback','createdata(''save'',gcf)',...
        'ListboxTop',0, ...
      'Position',[left bottom width height], ...
      'UserData',file,...
        'String','Save');

   % == Popup menus ===================================================

   % popup menu - class
   % title
   bottom=0.91;
   htxclass =uicontrol( ...
      'Style','text', ...
      'Units','normalized', ...
      'Position',[left bottom width 0.9*height], ...
      'String','Class');

   % popup menu
   for i=1:K,
      txnum=sprintf(' %d ',i);
      classes(i,1:size(txnum,2))=txnum;
   end

   bottom=bottom-height;
   hpuclass=uicontrol( ...
      'Style','popup', ...
      'Units','normalized', ...
      'Position',[left bottom width height], ...
      'String',classes);

   % == Edit lines =======================================================

   % x-axis
   bottom=bottom-1.2*height;
   htxxaxis=uicontrol( ...
      'Style','text', ...
      'Units','normalized', ...
      'Position',[left bottom width 0.9*height], ...
      'String','X-Axis');
   bottom=bottom-height;
   hedxaxis = uicontrol(...
    'Units','normalized', ...
      'ListboxTop',0, ...
        'Position',[left bottom width height], ...
      'CallBack','createdata(''setaxis'',gcf)',...
      'Style','edit',...
      'String','[-1 1]');

   % y-axis
   bottom=bottom-1.2*height;
   htxyaxis=uicontrol( ...
      'Style','text', ...
      'Units','normalized', ...
      'Position',[left bottom width 0.9*height], ...
      'String','Y-Axis');
   bottom=bottom-height;
   hedyaxis = uicontrol(...
    'Units','normalized', ...
      'ListboxTop',0, ...
      'Position',[left bottom width height], ...
      'CallBack','createdata(''setaxis'',gcf)',...
      'Style','edit',...
      'String','[-1 1]');

   % normal distributions are given by SIGMA in addition
   if strcmpi(varargin{1},'gauss'),

      % label
      bottom=bottom-1.5*height;
      htxcov=uicontrol( ...
         'Style','text', ...
         'Units','normalized', ...
         'Position',[left bottom width 0.9*height], ...
         'String','Covariance');

      % cov(xy)
      bottom=bottom-height;
      hedxx = uicontrol(...
        'Units','normalized', ...
         'ListboxTop',0, ...
         'Position',[left bottom width*0.5 height], ...
         'CallBack','createdata(''setcov'',gcf,1)',...
         'Style','edit',...
         'String','1');
      % cov(xy)
      hedxy = uicontrol(...
        'Units','normalized', ...
         'ListboxTop',0, ...
         'Position',[left+width*0.5 bottom width*0.5 height], ...
         'CallBack','createdata(''setcov'',gcf,2)',...
         'Style','edit',...
         'String','0');
      % cov(yx)
      bottom=bottom-height;
      hedyx = uicontrol(...
        'Units','normalized', ...
         'ListboxTop',0, ...
         'Position',[left bottom width*0.5 height], ...
         'CallBack','createdata(''setcov'',gcf,3)',...
         'Style','edit',...
         'String','0');
      % cov(yy)
      hedyy = uicontrol(...
        'Units','normalized', ...
         'ListboxTop',0, ...
         'Position',[left+width*0.5 bottom width*0.5 height], ...
         'CallBack','createdata(''setcov'',gcf,4)',...
         'Style','edit',...
         'String','1');

      bottom=bottom-1.2*height;
      htxmi1=uicontrol( ...
         'Style','text', ...
         'Units','normalized', ...
         'Position',[left bottom width 0.9*height], ...
         'String','MI=[');
      bottom=bottom-0.9*height;
      htxmi2=uicontrol( ...
         'Style','text', ...
         'Units','normalized', ...
         'Position',[left bottom width 0.9*height], ...
         'String','    ]');
   end

   % == Axes title ========================================================
   pos=get(haxes1,'Position');
   titletext=sprintf('File: %s',file.name);
   fontsize=(1-pos(2)-pos(4))*0.8;
   htitle=title(titletext,...
      'VerticalAlignment','bottom',...
      'HorizontalAlignment','left',...
      'FontUnits','normalized',...
      'Units','normalized',...
      'Position',[0 1 0],...
      'FontSize',fontsize);


   % =========================================================================

   % create the structures according to the current set type
   switch lower(varargin{1}),
   case 'finite'
      set(hfigure,'name','Generator of finite point sets');
      ident=ID_FINITE;
      % set handlers
      handlers=struct(...
         'settype',lower(varargin{1}),...
         'saved',1,...
         'btsave',hbtsave,...
         'edxaxis',hedxaxis,...
         'edyaxis',hedyaxis,...
         'title',htitle,...
         'btok',hbtok,...
         'axes1',haxes1,...
         'puclass',hpuclass );
   case 'gauss'
      set(hfigure,'name','Generator of Gaussian distributions');
      ident=ID_NORMAL;
      % set handlers
      handlers=struct(...
         'settype',lower(varargin{1}),...
         'currpoint',0,...
         'currhandle',0,...
         'saved',1,...
         'title',htitle,...
         'btsave',hbtsave,...
         'edxaxis',hedxaxis,...
         'edyaxis',hedyaxis,...
         'txmi1',htxmi1,...
         'txmi2',htxmi2,...
         'edxx',hedxx,...
         'edxy',hedxy,...
         'edyx',hedyx,...
         'edyy',hedyy,...
         'btok',hbtok,...
         'axes1',haxes1,...
         'puclass',hpuclass );
   end

   sets=struct(...
      'K',zeros(1,K),...
      'X',[],...
      'I',[],...
      'MI',[],...
      'SIGMA',[],...
      'N',2,...
      'id',ident);

   % store handlers and data set structure
   set(hfigure,'UserData',handlers);
   set(haxes1,'UserData',sets);


   % set figure as visible
   set(hfigure,'Visible','on');


case 'setcov'
   % == Set covariance matrix of current selected point ===================

   % get handlers
   hfigure=varargin{1};
   h=get(hfigure,'UserData');

   %get data set
   sets=get(h.axes1,'UserData');

   % current point
   i=h.currpoint;

   if varargin{2}==2,
      set(h.edyx,'String',get(h.edxy,'String'));
   elseif varargin{2}==3,
      set(h.edxy,'String',get(h.edyx,'String'));
   end

   % if some point is selected
   if i ~= 0,
      % get cov. matrix from edit lines
      sigma(1,1)=str2num(get(h.edxx,'String'));
      sigma(1,2)=str2num(get(h.edxy,'String'));
      sigma(2,1)=str2num(get(h.edyx,'String'));
      sigma(2,2)=str2num(get(h.edyy,'String'));

      % is sigma positive definite ?
      [aa,p]=chol(sigma);
      if p ~= 0,
         set(h.edxx,'String','1');
         set(h.edxy,'String','0');
         set(h.edyx,'String','0');
         set(h.edyy,'String','1');
         sigma=eye(2,2);
      end

      sets.SIGMA(:,(i-1)*2+1:i*2)=sigma;
      set(h.axes1,'UserData',sets);

%%%      window=axis;
      window=getaxis(h.axes1);
      R=min([(window(2)-window(1)),(window(4)-window(3))])/20;
      i=h.currpoint;
      class=sets.I(i);
%%      isigma=inv(sigma);
      mi=sets.MI(:,i);
%%%      [x,y]=ellipse(isigma,20,R,mi);
      [x,y]=ellips(mi,inv(sigma),R);

      if h.currhandle==0,
         % draw new ellipse
         h.currhandle =fill(x,y,XCOLORS(mod(class-1,MAXCOLOR)+1),...
            'EraseMode','none',...
            'ButtonDownFcn','createdata(''click'',gcf)',...
            'Tag','ellipse',...
            'UserData',mi);
         set(hfigure,'UserData',h);
      else
         set(h.currhandle,'XData',x,'YData',y,'EraseMode','normal');
      end
   end % if i ~= 0,

case 'redraw'
   % == Redraw axes ===========================================================
   hfigure=varargin{1};
   h = get(hfigure,'UserData');      % get handlers

   children=get(h.axes1,'Children' );
   set(children,'EraseMode','normal','Visible','off');

   % get sets
   sets=get(h.axes1,'UserData');

   % set axes
   if strcmpi(sets.id,ID_FINITE)==1,
      maxs=max(sets.X');
      mins=min(sets.X');
   elseif strcmpi(sets.id,ID_NORMAL )==1,
      % set axes
      maxs=max(sets.MI');
      mins=min(sets.MI');
   end
   dx=min( (maxs(1)-mins(1)), 1 )*DX_SPACE;
   dy=min( (maxs(2)-mins(2)), 1 )*DY_SPACE;
   x1=round(mins(1)-dx);
   x2=round(maxs(1)+dx);
   y1=round(mins(2)-dy);
   y2=round(maxs(2)+dx);
   win=[x1 x2 y1 y2];
   axes(h.axes1);
   %%%   axis(win);
   setaxis(h.axes1,win);

   % redraw points or ellipses
   if strcmpi(sets.id,ID_FINITE)==1,
      % points
      for i=1:sum(sets.K),
         line(sets.X(1,i),sets.X(2,i), ...
            'LineStyle','none', ...
            'Marker','.', ...
            'Color',XCOLORS(mod(sets.I(i)-1,MAXCOLOR)+1), ...
            'MarkerSize',25, ...
            'ButtonDownFcn','createdata(''click'',gcf)',...
            'EraseMode','none',...
            'Tag','point');
      end

   elseif strcmpi(sets.id,ID_NORMAL )==1,
      R=min([(win(2)-win(1)),(win(4)-win(3))])/20;
      % ellipses
      for i=1:sum(sets.K),
         sigma=sets.SIGMA(:,(i-1)*2+1:i*2);
         mi=sets.MI(:,i);
%%         [x,y]=ellipse(isigma,20,R,mi);
         [x,y]=ellips(mi,inv(sigma),R);
         class=sets.I(i);

         fill(x,y,XCOLORS(mod(class-1,MAXCOLOR)+1),...
            'EraseMode','none',...
            'ButtonDownFcn','createdata(''click'',gcf)',...
            'Tag','ellipse',...
            'UserData',mi);
      end
   end % if strcmpi(...

   % set new values
   set(h.edxaxis,'String',sprintf('[%s]',num2str(win(1:2))) );
   set(h.edyaxis,'String',sprintf('[%s]',num2str(win(3:4))) );

   for i=1:max(size(sets.K)),
      txnum=sprintf(' %d ',i);
      classes(i,1:size(txnum,2))=txnum;
   end
   set(h.puclass,'String',classes);


case 'setaxis'
   % == Set axes according to edit line X-Axis and Y-Axis ==================

   hfigure=varargin{1};
   h = get(hfigure,'UserData');      % get handlers

   xaxis=str2num(get(h.edxaxis,'String'));
   yaxis=str2num(get(h.edyaxis,'String'));

   if size(xaxis) ~= [1 2] | xaxis(2) <= xaxis(1),
      errordlg('Bad X-Axis limits.','Error','modal');
   elseif size(yaxis) ~= [1 2] | yaxis(2) <= yaxis(1),
      errordlg('Bad Y-Axis limits.','Error','modal');
   else
      setaxis(h.axes1,[xaxis yaxis]);
%%%      axis([xaxis yaxis]);
   end



case 'click'
   % == Mouse click handler =================================================

   % get handlers
   hfigure=varargin{1};
   h=get(hfigure,'Userdata');

   % get current pointer position
   pointer=get(h.axes1,'CurrentPoint');

   % get data sets structure
   sets=get(h.axes1,'UserData');

   % gco object invoked this function
   hobject=gco;

   clicktype = lower(get(hfigure,'SelectionType'));
   switch clicktype

   case 'normal'
      % === Left mouse button =============
      % Creat new point

      %
      if ~strcmpi(get(hobject,'Tag'),'point') & ~strcmpi(get(hobject,'Tag'),'ellipse'),

         % set up saved flag
         h.saved=0;
         set(hfigure,'UserData',h);

         % get class number
         class=get(h.puclass,'Value');

         % set update data set
         sets.K(class)=sets.K(class)+1;
         sets.I(sum(sets.K))=class;

         switch h.settype
         case 'finite'
            sets.X=[sets.X,[pointer(1,1);pointer(1,2)]];
            line(pointer(1,1),pointer(1,2), ...
               'LineStyle','none', ...
               'Marker','.', ...
               'Color',XCOLORS(mod(class-1,MAXCOLOR)+1), ...
               'MarkerSize',25, ...
               'ButtonDownFcn','createdata(''click'',gcf)',...
               'EraseMode','none',...
               'Tag','point');
            % store data set
            set(h.axes1,'UserData',sets);
         case 'gauss'
            sets.MI=[sets.MI,[pointer(1,1);pointer(1,2)]];
            sets.SIGMA=[sets.SIGMA,eye(2,2)];
            set(h.txmi1,'String',sprintf('MI=[%.2f',pointer(1,1)));
            set(h.txmi2,'String',sprintf('    %.2f]',pointer(1,2)));

            % set cov matrix and draw ellipse
            h.currpoint=size(sets.MI,2);
            h.currhandle=0;
            set(hfigure,'UserData',h);
            set(h.axes1,'UserData',sets);
            createdata('setcov',hfigure,0);
         end

      end % if ~strcmpi(

   case 'open'
      % == Double click ===
      % get info about selected point
      if strcmpi(h.settype,'gauss')==1 & strcmpi(get(hobject,'Tag'),'ellipse'),

         % set selected point as currpoint
         pixelpos=get(hobject,'UserData');
         K=sum(sets.K);
         for i=1:K,
            if pixelpos==sets.MI(:,i),
               h.currpoint=i;
               h.currhandle=hobject;
               sigma=sets.SIGMA(:,(i-1)*2+1:i*2);
               mi=sets.MI(:,i);

               % mi
               set(h.txmi1,'String',sprintf('MI=[%.2f',mi(1)));
               set(h.txmi2,'String',sprintf('    %.2f]',mi(2)));

               % show cov. matrix
               set(h.edxx,'String',sigma(1,1));
               set(h.edxy,'String',sigma(1,2));
               set(h.edyx,'String',sigma(2,1));
               set(h.edyy,'String',sigma(2,2));
               set(hfigure,'UserData',h);
               break;
            end
         end

      end % if ~strcmpi(get(...

   case 'alt'
      % === Right mouse button  ===
      % erase point

      if strcmpi(get(hobject,'tag'),'point')==1,

         % object is point
         pixelpos=[get(hobject,'XData');get(hobject,'YData')];

         K=sum(sets.K);
         for i=1:K,
            pos=sets.X(:,i);

            if pixelpos==pos,
               % update data sets
               sets.K(sets.I(i))=sets.K(sets.I(i))-1;
               sets.I=[sets.I(:,1:i-1),sets.I(:,i+1:K)];
               sets.X=[sets.X(:,1:i-1),sets.X(:,i+1:K)];
               set(h.axes1,'UserData',sets);

               % erase point
               set(hobject,'EraseMode','normal');
               delete(hobject);

               % set up saved flag
               h.saved=0;
               set(hfigure,'UserData',h);

               break;
            end % if pixelpos
         end % for

      elseif strcmpi(get(hobject,'tag'),'ellipse')==1,

         % object is ellipse
         pixelpos=get(hobject,'UserData');

         K=sum(sets.K);
         for i=1:K,
            pos=sets.MI(:,i);

            if pixelpos==pos,
               % update data sets
               sets.K(sets.I(i))=sets.K(sets.I(i))-1;
               sets.I=[sets.I(:,1:i-1),sets.I(:,i+1:K)];
               sets.MI=[sets.MI(:,1:i-1),sets.MI(:,i+1:K)];
               sets.SIGMA=[sets.SIGMA(:,1:(i-1)*2),sets.SIGMA(:,i*2+1:K*2)];
               h.currpoint=0;
               h.currhandle=0;

               set(h.axes1,'UserData',sets);

               % erase point
               set(hobject,'EraseMode','normal');
               delete(hobject);

               % set up saved flag
               h.saved=0;
               set(hfigure,'UserData',h);

               break;
            end % if pixelpos
         end % for i=1:K,
      end % if strcmpi(get(hpixel,...

   end % switch lower(get...

case 'load'
   % == Invokes stadard load file window ====================================
   % loads data from file and then puts them on the desktop.
   hfigure=varargin{1};
   h=get(hfigure,'UserData');

   % get data sets structure
   sets=get(h.axes1,'UserData');

   loadit=1;

   if h.saved==0,
      answer=questdlg(...
         'Data set was changed. Do you want to continue?',...
         'Warning',...
         'Continue','Cancel','Cancel');

      if answer==0 | strcmpi(answer,'Cancel'),
         loadit=0;
      end
   end

   if loadit==1,
      [name,path]=uigetfile('*.mat','Load data set');

      if name ~= 0,
         pathname=strcat(path,name);
         if strcmpi(sets.id, ID_FINITE), a = check2ddata(pathname); end
         if strcmpi(sets.id, ID_NORMAL), a = check2dgauss(pathname); end
           
%         if checkdat(pathname,sets.id,sets.N,0)==1,
         if a,

            % store path
            file.name=name;
            file.path=path;
            file.pathname=pathname;
            set(h.btsave,'UserData',file);

            oldID=sets.id;
            % load data set and store it
            sets=load(file.pathname);
            
            sets.id = oldID;
            sets.I = sets.y;
            sets.N=2;
            for ii=1:max(sets.I),
              sets.K(ii)=length(find(sets.I==ii));
            end
            if strcmpi(sets.id, ID_NORMAL),
              sets.MI = sets.Mean;
              sets.SIGMA=[];
              for ii=1:size(sets.Mean,2),
                sets.SIGMA=[sets.SIGMA, sets.Cov(:,:,ii)];
              end
            end
            
            set(h.axes1,'UserData',sets);

            % set everything possible...
            h.currpoint=0;
            h.currhandle=0;
            h.saved=1;
            set(hfigure,'UserData',h);

            % and put it on the desktop
            createdata('redraw',hfigure);

            % print title
            titletext=sprintf('File: %s',file.name);
            set(h.title,'String',titletext);

         else
            errordlg('This file does not contain required data.','Bad file','modal');
         end
      end % if name ~= 0,
   end % if loadit==1,


case 'save'
   % == Invokes standard save file window ====================================
   % and prepares data for saving.

   % get handlers
   hfigure=varargin{1};
   h=get(hfigure,'UserData');

   % get data set
   sets=get(h.axes1,'UserData');

   % is there something to save ?
   if sum(sets.K)~=0,
      % get current file name
      file=get(h.btsave,'UserData');

      % invoke save window
      [name,path]=uiputfile(file.pathname,'Save file');

      % Have user pushed down close button or save button ?
      if name ~= 0,
         % store file name
         file.name=name;
         file.path=path;
         file.pathname=strcat(path,name);
         set(h.btsave,'UserData',file);

         % print title
         titletext=sprintf('File: %s',file.name);
         set(h.title,'String',titletext);

         % remove classes which have zero length
         ssets=sets;
         KL=size(sets.K,2);
         NK=sum(sets.K);

         % following vars are to be saved
         id=sets.id;
         I=sets.I;
         K=sets.K;
         N=sets.N;

         % remove zero's clases
         i=1;
         while i <= KL,
            if K(i)==0,
               for j=1:NK,
                  if I(j) > i,
                     I(j)=I(j)-1;
                  end
               end

               K=[K(1:i-1),K(i+1:KL)];
               KL=KL-1;
            else
               i=i+1;
            end
         end

         % save data set to the file
         switch h.settype
         case 'finite'
            X=sets.X;
            y=I;
            save(file.pathname,'X','y');
         case 'gauss'
            Mean=sets.MI;
            y=I;
            for jj=1:size(Mean,2),
              Cov(:,:,jj)=sets.SIGMA(:,(jj-1)*2+1:jj*2);
            end
            save(file.pathname,'Mean','Cov','y');
         end

         % set up saved flag
         h.saved=1;
         set(hfigure,'UserData',h);
      end % if name ~= 0,
   end % if sum(sets.K)~=0,

case 'ok'
   % == OK button handler ===================================================

   % get nadlers
   hfigure=varargin{1};
   h=get(hfigure,'UserData');

   % is dialog to be closed ?
   closeit=1;

   if h.saved==0,
      % data have not saved.
      answer=questdlg(...
         'Data set was changed. Do you want to save data?',...
         'Warning',...
         'Save','Don`t save','Cancel','Cancel');

      % decide along to the answer
      if answer ~= 0,
         switch lower(answer)
         case 'save'
            createdata('save',hfigure);
            h=get(hfigure,'UserData');
            closeit=h.saved;
         case 'cancel'
            closeit=0;
         case 'don`t save'
            close(hfigure,hfigure);
            return;
      %      closeit=1;
         end

      else
         closeit=0;
      end % if answer~=0,...else

   end % if h.saved==0,

   % close dialog
   if closeit==1,
      % if some data was created invoke function given in arguments
      sets=get(h.axes1,'UserData');
      command=get(h.btok,'UserData');
      file=get(h.btsave,'UserData');

      if sum(sets.K)~=0 & size(command,2) > 2,
         feval(command{3},command{4:size(command,2)},file.path,file.name);
      end

      close(hfigure);
   end

   case 'close'
   % == Close button handler ===================================================

   % get nadlers
   hfigure=varargin{1};
   h=get(hfigure,'UserData');

   closeit=1;

   % data set changed ?
   if h.saved==0,
      % data have not saved.
      answer=questdlg(...
         'Data set was changed. Do you want to close?',...
         'Warning',...
         'Close','Cancel','Cancel');

      % decide along to the answer
      if answer == 0 | strcmpi(answer,'Cancel')==1,
         closeit=0;
      end
   end

   if closeit==1,
      close(hfigure);
   end

case 'info'
   % == Call standard Matlab info box =========================================
   helpwin(mfilename);

end

return;

%========================================                                       
function [rect]=getaxis(handle)                                                 
% function [rect]=getaxis(handle)                                               
%                                                                               
% GETAXIS returns a row vector containing the scaling for                       
%   the plot with a given handle.                                               
%                                                                               
% See also AXIS.                                                                
%                                                                               
                                                                                
rect=[get(handle,'XLim'),get(handle,'YLim'),get(handle,'ZLim')];                
                                                                                
return; 


function []=setaxis(handle,rect)                                                
% function []=setaxis(handle,rect)                                              
%                                                                               
% SETAXIS sets scaling for the x- and y-axes                                    
%   on the plot with a given handle.                                            
%                                                                               
% See also AXIS.                                                                
%                                                                               
                                                                                
set(handle,'XLim',rect(1:2));                                                   
set(handle,'YLim',rect(3:4));                                                   
                                                                                
if size(rect,2)>=6,                                                             
   set(handle,'ZLim',rect(5:6));                                                
end                                                                             
                                                                                
return;      