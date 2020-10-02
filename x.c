
    min_dt = processes[0].dt;

    for (i = 0; i < process_count && processes[i].t0 == sec; i++) {
        current = &processes[i];
        for(j = i + 1; j < process_count && processes[j].t0 == sec; j++) {
            if (processes[j].dt < min_dt) {
                min_dt = processes[j].dt;
                processes[i] = processes[j];
                processes[j] = *current;
            }
        }
        sec++;
    }

    for(i = 0; i < process_count; i++)
        printf("%s\n", processes[i].name);