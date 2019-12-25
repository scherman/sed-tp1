import pandas as pd
import os

def parse_output(output):
    df = pd.DataFrame()
    for line in output:
        line = line.replace('\n', '')
        parsed_line = list(filter(lambda x: x.strip()!='', line.split(' ')))
        df = df.append({"time": parsed_line[0], 'port': parsed_line[1], 'value': (parsed_line[2])}, ignore_index=True)
    return df

def process_file(f, filename):
    lines = f.readlines()
    df = parse_output(lines)
    df['file'] = filename
    
    # agrupo por time, file
    df = df.groupby(['time', 'file']).apply(lambda x: dict(x.values)).reset_index()

    # mapeo los valores en forma de dict a columnas
    df = df.drop([0], axis=1).join(df[0].apply(pd.Series))

    return df

def fetch_sim_data(dir_prefix):
    df = pd.DataFrame()
    for file in os.listdir():
        if (not file.startswith(dir_prefix)):
            continue
        print("Processing file {}".format(file))
        with open(file + '/output', 'r') as f:
            current_df = process_file(f, file)
            df = df.append(current_df)

    df['sun_degree'] = df['sun_degree'].apply(lambda x: float(x))
    df['sun_radiation'] = df['sun_radiation'].apply(lambda x: float(x))

    return df