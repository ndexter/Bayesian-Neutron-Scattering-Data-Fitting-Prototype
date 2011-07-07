function plotcom(filename1, filename2, filename3)

    clf;

    fid00 = mopen(filename1, 'r');
    fid01 = mopen(filename2, 'r');
    fid02 = mopen(filename3, 'r');
    
    if(fid00 == -1)
        error('cannot open datafile');
    end
    if(fid01 == -1)
        error('cannot open datafile');
    end
    if(fid02 == -1)
        error('cannot open datafile');
    end

    xselect();
    f=gcf();
    f.pixmap='on';

    x_col=1;
    d_col=2;
    e_col=3;

    // loop over all lines in the file, reading them one at a time
    num_lines = 0;
    done_yet = 0;
    data_done = 0;
    n = 0;

    previous = 0;

    while(done_yet == 0)

        if(n < 100)
            n = n + 1;
        elseif(n >= 100 & n < 10000)
            n = n + 100;
        elseif(n >= 10000 & n < 100000)
            n = n + 1000;
        elseif(n >= 100000)
            n = n + 10000;
        end

        if(data_done == 0)
            [num_read00, val00(1), val00(2)] = mfscanf(fid00, "%f %f");
            [num_read01, val01(1), val01(2), val01(3)] = mfscanf(fid01, "%f %f %f");
            [num_read02, val02(1), val02(2), val02(3)] = mfscanf(fid02, "%f %f %f");
        end

        while(data_done == 0)

            if (num_read00 ~= 2)
                data_done = 1;
                break;
            end
            if (num_read01 ~= 3)
                done_yet = 1;
                break;
            end
            if (num_read02 ~= 3)
                done_yet = 1;
                break;
            end

            num_lines = num_lines + 1;
            a_array00(num_lines) = val00(x_col);
            d_array00(num_lines) = val00(d_col);
            d_array01(num_lines) = val01(d_col);
            e_array01(num_lines) = val01(e_col);
            d_array02(num_lines) = val02(d_col);
            e_array02(num_lines) = val02(e_col);

            [num_read00, val00(1), val00(2)] = mfscanf(fid00, "%f %f");
            [num_read01, val01(1), val01(2), val01(3)] = mfscanf(fid01, "%f %f %f");
            [num_read02, val02(1), val02(2), val02(3)] = mfscanf(fid02, "%f %f %f");

            if(num_read00 <= 0)

                data_done = 1;

                clf();
                
                // Plot time vs states
                plot2d(a_array00, [d_array00, d_array01, d_array02, e_array01, e_array02],..
                    leg="", rect=[a_array00(1),0,a_array00(193),0.250]);
                
                // Set labels and title
                xlabel('Energy Transfer (meV)');
                ylabel('Intensity(a.u.)');
                title('Spectra of TiCr1.85H0.45 measured on TFXA');
                //xstring(218*0.9,0.210,'n = ' + string(n));
                
                // Get axes and children
                a = gca();
                a.isoview = 'on'; 
                a.children; 

                data = a.children.children(6);
                data.foreground = 1;
                data.thickness = 1;
                data.line_style = 1;
                data.line_mode = "off";
                data.mark_mode = "on";
                data.mark_style = 1;
                model1 = a.children.children(5);
                model1.foreground = 2;
                model1.thickness = 2;
                model1.line_style = 1;
                model2 = a.children.children(4);
                model2.foreground = 3;
                model2.thickness = 2;
                model2.line_style = 1;
                err1 = a.children.children(3);
                err1.foreground = 5;
                err1.thickness = 1;
                err1.line_style = 1;
                err2 = a.children.children(2);
                err2.foreground = 6;
                err2.thickness = 1;
                err2.line_style = 1;
                legends(['n = ' + string(n);'Data';'Model Chi'; 'Model Bayes';'Error Chi';'Error Bayes'],..
                        [[0;0],[1;1],[2;1],[3;1],[5;1],[6;1]],opt="ur")
                a.isoview='off';
                show_pixmap();

                num_lines = 0;
                previous = 0;

                break;
            else
                previous = val00(x_col)
            end
        end

        clf();
        
        // Plot time vs states
        plot2d(a_array00, [d_array00, d_array01, d_array02, e_array01, e_array02],..
            leg="", rect=[a_array00(1),0,a_array00(193),0.250]);
        
        // Set labels and title
        xlabel('Energy Transfer (meV)');
        ylabel('Intensity(a.u.)');
        title('Spectra of TiCr1.85H0.45 measured on TFXA');
        //xstring(218*0.9,0.210,'n = ' + string(n));
        
        // Get axes and children
        a = gca();
        a.isoview = 'on'; 
        a.children; 

        data = a.children.children(6);
        data.foreground = 1;
        data.thickness = 1;
        data.line_style = 1;
        data.line_mode = "off";
        data.mark_mode = "on";
        data.mark_style = 1;
        model1 = a.children.children(5);
        model1.foreground = 2;
        model1.thickness = 2;
        model1.line_style = 1;
        model2 = a.children.children(4);
        model2.foreground = 3;
        model2.thickness = 2;
        model2.line_style = 1;
        err1 = a.children.children(3);
        err1.foreground = 5;
        err1.thickness = 1;
        err1.line_style = 1;
        err2 = a.children.children(2);
        err2.foreground = 6;
        err2.thickness = 1;
        err2.line_style = 1;
        legends(['n = ' + string(n);'Data';'Model Chi'; 'Model Bayes';'Error Chi';'Error Bayes'],..
                [[0;0],[1;1],[2;1],[3;1],[5;1],[6;1]],opt="ur")
        a.isoview='off';
        show_pixmap();

        num_lines = 0;

        while(done_yet == 0)

            if (num_read01 ~= 3)
                done_yet = 1;
                break;
            end
            if (num_read02 ~= 3)
                done_yet = 1;
                break;
            end

            num_lines = num_lines + 1;
            d_array01(num_lines) = val01(d_col);
            e_array01(num_lines) = val01(e_col);
            d_array02(num_lines) = val02(d_col);
            e_array02(num_lines) = val02(e_col);

            [num_read01, val01(1), val01(2), val01(3)] = mfscanf(fid01, "%f %f %f");
            [num_read02, val02(1), val02(2), val02(3)] = mfscanf(fid02, "%f %f %f");

            if(val01(x_col) < previous)
                previous = 0;
                break;
            else
                previous = val01(x_col);
            end
        end

    end

    mclose(fid00);
    mclose(fid01);
    mclose(fid02);

endfunction
